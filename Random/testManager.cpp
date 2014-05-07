#include <SSVUtils/SSVUtils.hpp>

using Idx = std::size_t;
using Ctr = int;

template<typename> class Manager;

namespace Internal
{
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

		public:
			enum class State : int {Alive = 0, Unused = 1, Dead = 2};

		private:
			Idx markIdx;
			State state{State::Unused};
			Uncertain<T> data;

			// Initializes the internal data
			template<typename... TArgs> inline void initData(TArgs&&... mArgs) 
				noexcept(noexcept(data.init(std::forward<TArgs>(mArgs)...)))
			{
				SSVU_ASSERT(state == State::Unused);
				data.init(std::forward<TArgs>(mArgs)...);
			}

			// Deinitializes the internal data
			inline void deinitData() noexcept(noexcept(data.deinit()))
			{ 
				SSVU_ASSERT(state != State::Unused);
				data.deinit();
			}

		public:
			inline Atom() = default;
			inline Atom(Atom&&) = default;
			inline Atom& operator=(Atom&&) = default;

			inline T& getData() noexcept 				{ SSVU_ASSERT(state != State::Unused); return data.get(); }				
			inline const T& getData() const noexcept 	{ SSVU_ASSERT(state != State::Unused); return data.get(); }	
			inline void setDead() noexcept 				{ state = State::Dead; }
			
			// Disallow copies
			inline Atom(const Atom&) = delete;
			inline Atom& operator=(const Atom&) = delete;
	};
}

template<typename T> class Handle
{
	template<typename> friend class Manager;

	public:
		using AtomType = typename Internal::Atom<T>;

	private:
		Manager<T>& manager;
		Idx markIdx;
		Ctr ctr;

		inline Handle(Manager<T>& mManager, Idx mCtrlIdx, Ctr mCtr) noexcept 
			: manager(mManager), markIdx{mCtrlIdx}, ctr{mCtr} { }		

		template<typename TT> inline TT getAtomImpl() noexcept
		{
			SSVU_ASSERT(isAlive());
			return manager.getAtomFromMark(manager.marks[markIdx]);
		}
		
	public:
		inline AtomType& getAtom() noexcept 			{ return getAtomImpl<AtomType&>(); }
		inline const AtomType& getAtom() const noexcept { return getAtomImpl<const AtomType&>(); }
		inline T& get() noexcept						{ return getAtom().getData(); }
		inline const T& get() const noexcept			{ return getAtom().getData(); }
		bool isAlive() const noexcept;
		void destroy() noexcept;
};

template<typename T> class Manager
{
	template<typename> friend class Handle;

	private:
		struct Mark { Idx idx; Ctr ctr; };

	public:
		using AtomType = typename Internal::Atom<T>;
		using AtomState = typename AtomType::State;

	private:
		std::vector<AtomType> atoms;
		std::vector<Mark> marks;
		Idx size{0u}, next{0u};

		inline std::size_t getCapacity() const noexcept { return atoms.size(); }	

		inline void growCapacity(std::size_t mAmount)
		{
			auto oldSize(getCapacity()), newSize(oldSize + mAmount);
			SSVU_ASSERT(newSize >= 0 && newSize >= oldSize);

			atoms.resize(newSize);
			marks.resize(newSize);

			// Initialize resized storage
			for(auto i(oldSize); i < newSize; ++i) atoms[i].markIdx = marks[i].idx = i;									
		}

		inline void growIfNeeded()
		{
			constexpr std::size_t growAmount{10};
			if(getCapacity() <= next) growCapacity(growAmount);
		}

		inline void destroy(Idx mCtrlIdx) noexcept
		{			
			getAtomFromMark(marks[mCtrlIdx]).setDead();
		}

		inline Mark& getMarkFromAtom(const AtomType& mAtom)	{ return marks[mAtom.markIdx]; }
		inline AtomType& getAtomFromMark(const Mark& mMark)	{ return atoms[mMark.idx]; }

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
			marks.clear();
			size = next = 0u;
		}

		inline void reserve(std::size_t mCapacity) { if(getCapacity() < mCapacity) growCapacity(mCapacity); }

		template<typename... TArgs> inline Handle<T> createAtom(TArgs&&... mArgs)
		{
			// `next` may be greater than the sizes of the vectors - resize vectors if needed 
			growIfNeeded();

			// `next` now is the first empty valid index - we create our atom there
			atoms[next].initData(std::forward<TArgs>(mArgs)...);
			atoms[next].state = AtomState::Alive;

			// Update the mark
			auto cIdx(atoms[next].markIdx);
			auto& mark(marks[cIdx]);
			mark.idx = next;
			++mark.ctr;

			// Update next free index
			++next;

			return {*this, cIdx, mark.ctr};	
		}	

		inline void refresh()
		{
			// C++14: use polymorphic lambda
			ssvu::sortStable(atoms, [](const AtomType& mA, const AtomType& mB){ return mA.state < mB.state; });

			// Starting from the end, update dead entities and their marks
			auto i(atoms.size() - 1);			
			for(; i > 0 && atoms[i].state == AtomState::Dead; --i)				
			{
				atoms[i].deinitData();
				atoms[i].state = AtomState::Unused;
				++(getMarkFromAtom(atoms[i]).ctr);				
			}
						
			// Starting from the beginning, update alive entities and their marks
			for(i = 0u; i <= atoms.size() && atoms[i].state == AtomState::Alive; ++i) 			
				getMarkFromAtom(atoms[i]).idx = i;
			
			// Update size and next free index
			size = next = i;
		}

		template<typename TFunc> inline void forEach(TFunc mFunc)
		{
			for(auto i(0u); i < size; ++i) mFunc(atoms[i].getData());
		}	
		template<typename TFunc> inline void forEachAtom(TFunc mFunc)
		{
			for(auto i(0u); i < size; ++i) mFunc(atoms[i]);
		}

		inline AtomType& getAtomAt(Idx mIdx) noexcept 				{ SSVU_ASSERT(mIdx < atoms.size()); return atoms[mIdx]; }
		inline const AtomType& getAtomAt(Idx mIdx) const noexcept 	{ SSVU_ASSERT(mIdx < atoms.size()); return atoms[mIdx]; }
		inline T& getDataAt(Idx mIdx) noexcept 						{ return getAtomAt(mIdx).getData(); }
		inline const T& getDataAt(Idx mIdx) const noexcept 			{ return getAtomAt(mIdx).getData(); }

		inline std::size_t getSizeCurrent() const noexcept 	{ return size; }
		inline std::size_t getSizeNext() const noexcept 	{ return next; }

		void printState()
		{
			ssvu::lo("ATOMS") << "";
			for(const auto& a : atoms) std::cout << std::setw(4) << std::left << (int)a.state << " ";
			std::cout << "\n";

			ssvu::lo("CTIDX") << "";
			for(const auto& a : marks) std::cout << std::setw(4) << std::left << (int)a.idx << " ";
			std::cout << "\n";

			ssvu::lo("CTCTR") << "";
			for(const auto& a : marks) std::cout << std::setw(4) << std::left << (int)a.ctr << " ";
			std::cout << "\n\n";

			ssvu::lo("ASTRS") << "\n";
			std::size_t idx{0u};
			//forEach([&idx](std::string& mS){ std::cout << idx++ << ": " << mS << "\n"; });
			std::cout << std::endl;
		}
};

template<typename T> inline bool Handle<T>::isAlive() const noexcept
{ 
	return manager.marks[markIdx].ctr == ctr;
}

template<typename T> inline void Handle<T>::destroy() noexcept
{ 
	return manager.destroy(markIdx);
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

			test.refresh();

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

			test.refresh();

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