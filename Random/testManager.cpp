#include <SSVUtils/SSVUtils.hpp>

using Idx = std::size_t;
using Ctr = int;

template<typename> class Manager;
template<typename> class Handle;

template<typename T> class Atom
{
	template<typename> friend class Manager;
	template<typename> friend class Handle;

	private:
		Idx ctrlIdx;
		bool alive{false};
		T impl;

	public:
		template<typename... TArgs> inline Atom(Idx mCtrlIdx, TArgs&&... mArgs) 
			: ctrlIdx{mCtrlIdx}, impl{std::forward<TArgs>(mArgs)...} { }
};

class Controller
{
	template<typename> friend class Manager;
	template<typename> friend class Handle;

	private:
		Idx idx;
		Ctr ctr;

	public:
		inline Controller(Idx mIdx) noexcept : idx{mIdx} { }
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
		
		Atom<T>& getAtom();

	public:
		T& get();
		bool isAlive();
		void destroy();
};

template<typename T> class Manager
{
	template<typename> friend class Handle;

	private:
		std::vector<Atom<T>> atoms;
		std::vector<Controller> controllers;
		Idx lastFree{0u};

		inline void checkResize()
		{
			constexpr std::size_t resizeAmount{10};

			// If the last free index is valid, return
			auto oldSize(atoms.size());
			if(oldSize > lastFree) return;
			
			// Calculate new size and reserve required memory
			auto newSize(oldSize + resizeAmount);
			atoms.reserve(newSize);
			controllers.reserve(newSize);

			// Initialize reserved memory
			while(oldSize++ < newSize)
			{
				atoms.emplace_back(oldSize);					
				controllers.emplace_back(oldSize);
			}
		}

		inline void destroy(Idx mCtrlIdx)
		{
			atoms[controllers[mCtrlIdx].idx].alive = false;
		}

	public:
		inline Manager() { }

		template<typename... TArgs> inline Handle<T> createAtom(TArgs&&... mArgs)
		{
			checkResize();

			atoms[lastFree].impl = T(std::forward<TArgs>(mArgs)...);
			atoms[lastFree].alive = true;

			auto cIdx(atoms[lastFree].ctrlIdx);
			controllers[cIdx].idx = lastFree;
			++(controllers[cIdx].ctr);
			++lastFree;

			return {*this, cIdx, controllers[cIdx].ctr};	
		}	

		inline void refresh()
		{
			// C++14: use polymorphic lambda
			ssvu::sortStable(atoms, [](const Atom<T>& mA, const Atom<T>& mB){ return mA.alive > mB.alive; });

			auto rIdx(atoms.size() - 1);
			for(; !atoms[rIdx].alive && rIdx > 0; --rIdx)
			{
				auto& controller(controllers[atoms[rIdx].ctrlIdx]);
				++(controller.ctr);
				controller.idx = -1;
			}

			for(auto fIdx(0u); fIdx <= rIdx; ++fIdx) controllers[atoms[fIdx].ctrlIdx].idx = fIdx;
			lastFree = rIdx + 1; // ? check
		}

		void printState()
		{
			ssvu::lo("ATOMS") << "";
			for(const auto& a : atoms) std::cout << std::setw(4) << std::left << (int)a.alive << " ";
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

template<typename T> inline Atom<T>& Handle<T>::getAtom()
{ 
	SSVU_ASSERT(isAlive());
	return manager.atoms[manager.controllers[ctrlIdx].idx];
}

template<typename T> inline T& Handle<T>::get()
{ 	
	return getAtom().impl;
}

template<typename T> inline bool Handle<T>::isAlive()
{ 
	return manager.controllers[ctrlIdx].ctr == ctr;
}

template<typename T> inline void Handle<T>::destroy()
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

	return 0;
}