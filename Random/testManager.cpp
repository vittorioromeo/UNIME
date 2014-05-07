#include <SSVUtils/SSVUtils.hpp>

using Idx = std::size_t;
using Ctr = int;

enum class AtomState : int {Alive = 0, Unused = 1, Dead = 2};

template<typename> class Manager;
template<typename> class Handle;

template<typename T> class Atom
{
	SSVU_ASSERT_STATIC(ssvu::isDefaultConstructible<T>(), "T must be default-constructible");

	template<typename> friend class Manager;
	template<typename> friend class Handle;

	private:
		Idx ctrlIdx;
		AtomState state{AtomState::Unused};
		T impl;

		template<typename... TArgs> inline void initImpl(TArgs&&... mArgs) 
			noexcept(noexcept(T(std::forward<TArgs>(mArgs)...)))
		{
			impl = T(std::forward<TArgs>(mArgs)...);
		}

	public:
		inline T& getImpl() noexcept 	{ return impl; }
		inline void destroy() noexcept 	{ state = AtomState::Dead; }
};

class Controller
{
	template<typename> friend class Manager;
	template<typename> friend class Handle;

	private:
		Idx idx;
		Ctr ctr;
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
		
	public:
		Atom<T>& getAtom() noexcept;
		T& get() noexcept;
		bool isAlive() const noexcept;
		void destroy() noexcept;
};

template<typename T> class Manager
{
	template<typename> friend class Handle;

	private:
		std::vector<Atom<T>> atoms;
		std::vector<Controller> controllers;
		Idx size{0u};

		inline void resizeStorage(std::size_t mOldSize, std::size_t mNewSize)
		{
			SSVU_ASSERT(mNewSize >= 0);

			atoms.resize(mNewSize);
			controllers.resize(mNewSize);

			// Initialize resized storage
			for(; mOldSize < mNewSize; ++mOldSize)
			{
				atoms[mOldSize].ctrlIdx = mOldSize;					
				controllers[mOldSize].idx = mOldSize;
			}
		}

		inline void resizeIfNeeded()
		{
			constexpr std::size_t resizeAmount{10};

			// If the first free index is valid, return
			auto oldSize(atoms.size());
			if(oldSize > size) return;
			
			// Calculate new size and resize storage
			resizeStorage(oldSize, oldSize + resizeAmount);
		}

		inline void destroy(Idx mCtrlIdx) noexcept
		{			
			getAtomFromController(controllers[mCtrlIdx]).destroy();
		}

		inline Controller& getControllerFromAtom(const Atom<T>& mAtom)
		{
			return controllers[mAtom.ctrlIdx];
		}
		inline Atom<T>& getAtomFromController(const Controller& mController)
		{
			return atoms[mController.idx];
		}

	public:
		inline Manager() { }

		inline void clear() noexcept
		{
			atoms.clear();
			controllers.clear();
			size = 0;
		}

		inline void reserve(std::size_t mSize) 
		{ 
			resizeStorage(atoms.size(), mSize); 
		}

		template<typename... TArgs> inline Handle<T> createAtom(TArgs&&... mArgs)
		{
			// `size` may be greater than the sizes of the vectors - resize vectors if needed 
			resizeIfNeeded();

			// `size` now is the first empty valid index - we create our atom there
			atoms[size].initImpl(std::forward<TArgs>(mArgs)...);
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
			for(; atoms[i].state == AtomState::Dead && i > 0; --i)				
			{
				atoms[i].state = AtomState::Unused;
				++(getControllerFromAtom(atoms[i]).ctr);				
			}
						
			// Starting from the beginning, update alive entities and their controllers
			for(i = 0u; atoms[i].state == AtomState::Alive && i <= atoms.size(); ++i) 			
				getControllerFromAtom(atoms[i]).idx = i;
			
			// Update current size
			size = i;
		}

		template<typename TFunc> inline void forEach(const TFunc& mFunc)
		{
			for(auto i(0u); i < size; ++i) mFunc(atoms[i].impl);
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
			for(const auto& a : atoms) std::cout << idx++ << ": " << a.impl << "\n";
			std::cout << std::endl;
		}
};

template<typename T> inline Atom<T>& Handle<T>::getAtom() noexcept
{ 
	SSVU_ASSERT(isAlive());
	return manager.getAtomFromController(manager.controllers[ctrlIdx]);
}

template<typename T> inline T& Handle<T>::get() noexcept
{ 	
	return getAtom().getImpl();
}

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
	Manager<std::string> test;

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
	//test.atoms[2].alive = false;
	//test.atoms[3].alive = false;
	//test.atoms[5].alive = false;

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

	return 0;
}