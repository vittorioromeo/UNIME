#include <SSVUtils/SSVUtils.hpp>

using Idx = std::size_t;
using Ctr = int;

enum class AtomState : int {Alive = 0, Unused = 1, Dead = 2};

template<typename> class Manager;
template<typename> class Handle;

template<typename T> class Uncertain
{
	private:
		ssvu::AlignedStorageBasic<T> storage;

	public:
		template<typename... TArgs> inline void init(TArgs&&... mArgs) noexcept(ssvu::isNothrowConstructible<T>())
		{
			new (&storage) T(std::forward<TArgs>(mArgs)...);
		}
		inline void deinit() noexcept(ssvu::isNothrowDestructible<T>()) { get().~T(); }

		inline T& get() noexcept 				{ return reinterpret_cast<T&>(storage); }				
		inline const T& get() const noexcept 	{ return reinterpret_cast<const T&>(storage); }				
};

template<typename T> class Atom 
{
	template<typename> friend class Manager;

	private:
		Idx ctrlIdx;
		AtomState state{AtomState::Unused};
		Uncertain<T> data;

		// Initializes the internal data
		template<typename... TArgs> inline void initData(TArgs&&... mArgs) noexcept(ssvu::isNothrowConstructible<T>())
		{
			SSVU_ASSERT(state == AtomState::Unused);
			data.init(std::forward<TArgs>(mArgs)...);
		}

		// Deinitializes the internal data
		inline void deinitData() noexcept(ssvu::isNothrowDestructible<T>())
		{ 
			SSVU_ASSERT(state != AtomState::Unused);
			data.deinit();
		}

	public:
		inline Atom() = default;
		inline Atom(Atom&&) = default;
		inline Atom& operator=(Atom&&) = default;

		inline T& getData() noexcept 				{ SSVU_ASSERT(state != AtomState::Unused); return data.get(); }				
		inline const T& getData() const noexcept 	{ SSVU_ASSERT(state != AtomState::Unused); return data.get(); }	
		inline void setDead() noexcept 				{ state = AtomState::Dead; }
		
		// Disallow copies
		inline Atom(const Atom&) = delete;
		inline Atom& operator=(const Atom&) = delete;
};

template<typename T> class Handle
{
	template<typename> friend class Manager;

	private:
		Manager<T>& manager;
		Idx ctrlIdx;
		Ctr ctr;

		inline Handle(Manager<T>& mManager, Idx mCtrlIdx, Ctr mCtr) noexcept 
			: manager(mManager), ctrlIdx{mCtrlIdx}, ctr{mCtr} { }

		template<typename TT> inline TT getAtomImpl() noexcept
		{
			SSVU_ASSERT(isAlive());
			return manager.getAtomFromController(manager.controllers[ctrlIdx]);
		}
		
	public:
		inline Atom<T>& getAtom() noexcept 				{ return getAtomImpl<Atom<T>&>(); }
		inline const Atom<T>& getAtom() const noexcept 	{ return getAtomImpl<const Atom<T>&>(); }
		inline T& get() noexcept						{ return getAtom().getData(); }
		inline const T& get() const noexcept			{ return getAtom().getData(); }
		bool isAlive() const noexcept;
		void destroy() noexcept;
};

template<typename T> class Manager
{
	template<typename> friend class Handle;

	private:
		struct Controller { Idx idx; Ctr ctr; };

		std::vector<Atom<T>> atoms;
		std::vector<Controller> controllers;
		Idx size{0u};

		inline void growStorage(std::size_t mOldSize, std::size_t mNewSize)
		{
			SSVU_ASSERT(mNewSize >= 0 && mNewSize >= mOldSize);

			atoms.resize(mNewSize);
			controllers.resize(mNewSize);

			// Initialize resized storage
			for(; mOldSize < mNewSize; ++mOldSize)
			{
				atoms[mOldSize].ctrlIdx = mOldSize;					
				controllers[mOldSize].idx = mOldSize;
			}
		}

		inline void growIfNeeded()
		{
			constexpr std::size_t resizeAmount{10};

			// If the first free index is valid, return
			auto oldSize(atoms.size());
			if(oldSize > size) return;

			// Calculate new size and grow storage
			growStorage(oldSize, oldSize + resizeAmount);
		}

		inline void destroy(Idx mCtrlIdx) noexcept
		{			
			getAtomFromController(controllers[mCtrlIdx]).setDead();
		}

		inline Controller& getControllerFromAtom(const Atom<T>& mAtom)
		{
			return controllers[mAtom.ctrlIdx];
		}
		inline Atom<T>& getAtomFromController(const Controller& mController)
		{
			return atoms[mController.idx];
		}

		inline void cleanUpMemory()
		{
			for(auto& a : atoms) 				
				if(a.state != AtomState::Unused) 
				{
					a.deinitData();
					a.state = AtomState::Unused;
				}			
		}

	public:
		inline Manager() = default;
		inline ~Manager() { cleanUpMemory(); }

		inline void clear() noexcept
		{
			cleanUpMemory();
			atoms.clear();
			controllers.clear();
			size = 0;
		}

		inline void reserve(std::size_t mSize) 
		{ 
			growStorage(atoms.size(), mSize); 
		}

		template<typename... TArgs> inline Handle<T> createAtom(TArgs&&... mArgs)
		{
			// `size` may be greater than the sizes of the vectors - resize vectors if needed 
			growIfNeeded();

			// `size` now is the first empty valid index - we create our atom there
			atoms[size].initData(std::forward<TArgs>(mArgs)...);
			atoms[size].state = AtomState::Alive;

			// Update the controller
			auto cIdx(atoms[size].ctrlIdx);
			auto& controller(controllers[cIdx]);
			controller.idx = size;
			++controller.ctr;

			// Update current size
			++size;

			return {*this, cIdx, controller.ctr};	
		}	

		inline void refresh()
		{
			// C++14: use polymorphic lambda
			ssvu::sortStable(atoms, [](const Atom<T>& mA, const Atom<T>& mB){ return mA.state < mB.state; });

			// Starting from the end, update dead entities and their controllers
			auto i(atoms.size() - 1);			
			for(; i > 0 && atoms[i].state == AtomState::Dead; --i)				
			{
				atoms[i].deinitData();
				atoms[i].state = AtomState::Unused;
				++(getControllerFromAtom(atoms[i]).ctr);				
			}
						
			// Starting from the beginning, update alive entities and their controllers
			for(i = 0u; i <= atoms.size() && atoms[i].state == AtomState::Alive; ++i) 			
				getControllerFromAtom(atoms[i]).idx = i;
			
			// Update current size
			size = i;
		}

		template<typename TFunc> inline void forEach(TFunc mFunc)
		{
			for(auto i(0u); i < size; ++i) mFunc(atoms[i].getData());
		}	
		template<typename TFunc> inline void forEachAtom(TFunc mFunc)
		{
			for(auto i(0u); i < size; ++i) mFunc(atoms[i]);
		}	

		inline std::size_t getSize() const noexcept { return size; }

		void printState()
		{
			ssvu::lo("ATOMS") << "";
			for(const auto& a : atoms) std::cout << std::setw(4) << std::left << (int)a.state << " ";
			std::cout << "\n";

			ssvu::lo("CTIDX") << "";
			for(const auto& a : controllers) std::cout << std::setw(4) << std::left << (int)a.idx << " ";
			std::cout << "\n";

			ssvu::lo("CTCTR") << "";
			for(const auto& a : controllers) std::cout << std::setw(4) << std::left << (int)a.ctr << " ";
			std::cout << "\n\n";

			ssvu::lo("ASTRS") << "\n";
			std::size_t idx{0u};
			//forEach([&idx](std::string& mS){ std::cout << idx++ << ": " << mS << "\n"; });
			std::cout << std::endl;
		}
};

template<typename T> inline bool Handle<T>::isAlive() const noexcept
{ 
	return manager.controllers[ctrlIdx].ctr == ctr;
}

template<typename T> inline void Handle<T>::destroy() noexcept
{ 
	return manager.destroy(ctrlIdx);
}


int main()
{
	for(int j = 0; j < 2; ++j)
	{
		Manager<std::string> test;

		for(int k = 0; k < 2; ++k)
		{
			test.printState();

			auto a0 = test.createAtom();
			auto a1 = test.createAtom();
			auto a2 = test.createAtom();
			auto a3 = test.createAtom();
			auto a4 = test.createAtom();
			auto a5 = test.createAtom();
			auto a6 = test.createAtom();

			a0.get() = "hi";
			a4.get() = "ciao";
			a6.get() = "bye";

			test.printState();

			a2.destroy();
			a3.destroy();
			a5.destroy();

			test.printState();

			test.refresh();

			test.printState();

			ssvu::lo("RESULT") << a0.get() << std::endl;
			ssvu::lo("RESULT") << a4.get() << std::endl;
			ssvu::lo("RESULT") << a6.get() << std::endl;

			ssvu::lo("alive") << a0.isAlive() << std::endl;
			ssvu::lo("alive") << a1.isAlive() << std::endl;
			ssvu::lo("alive") << a2.isAlive() << std::endl;
			ssvu::lo("alive") << a3.isAlive() << std::endl;
			ssvu::lo("alive") << a4.isAlive() << std::endl;
			ssvu::lo("alive") << a5.isAlive() << std::endl;
			ssvu::lo("alive") << a6.isAlive() << std::endl;

			test.forEach([](std::string& mStr){ mStr += "bb"; });

			ssvu::lo("RESULT2") << a0.get() << std::endl;
			ssvu::lo("RESULT2") << a4.get() << std::endl;
			ssvu::lo("RESULT2") << a6.get() << std::endl;
		}		
	}

	struct TestRes 
	{
		std::string s;
		TestRes(std::string mS) : s(std::move(mS)) { }
	};

	for(int j = 0; j < 2; ++j)
	{
		Manager<TestRes> test;

		for(int k = 0; k < 2; ++k)
		{
			test.printState();

			auto a0 = test.createAtom("atom00");
			auto a1 = test.createAtom("atom01");
			auto a2 = test.createAtom("atom02");
			auto a3 = test.createAtom("atom03");
			auto a4 = test.createAtom("atom04");
			auto a5 = test.createAtom("atom05");
			auto a6 = test.createAtom("atom06");

			a0.get().s += " mod";
			a4.get().s += " mod";
			a6.get().s += " mod";

			test.printState();

			a2.destroy();
			a3.destroy();
			a5.destroy();

			test.printState();

			test.refresh();

			test.printState();

			ssvu::lo("RESULT") << a0.get().s << std::endl;
			ssvu::lo("RESULT") << a4.get().s << std::endl;
			ssvu::lo("RESULT") << a6.get().s << std::endl;

			ssvu::lo("alive") << a0.isAlive() << std::endl;
			ssvu::lo("alive") << a1.isAlive() << std::endl;
			ssvu::lo("alive") << a2.isAlive() << std::endl;
			ssvu::lo("alive") << a3.isAlive() << std::endl;
			ssvu::lo("alive") << a4.isAlive() << std::endl;
			ssvu::lo("alive") << a5.isAlive() << std::endl;
			ssvu::lo("alive") << a6.isAlive() << std::endl;

			test.forEach([](TestRes& mRes){ mRes.s += "bb"; });

			ssvu::lo("RESULT2") << a0.get().s << std::endl;
			ssvu::lo("RESULT2") << a4.get().s << std::endl;
			ssvu::lo("RESULT2") << a6.get().s << std::endl;
		}		
	}
	
	return 0;
}