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

		inline T& operator*() noexcept 				{ return get(); }
		inline const T& operator*() const noexcept 	{ return get(); }
		inline T* operator->() noexcept 			{ return &(get()); }
		inline const T* operator->() const noexcept { return &(get()); }
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
		Idx size{0u}, sizeNext{0u};

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
			if(getCapacity() <= sizeNext) growCapacity(growAmount);
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
			{
				if(a.state != AtomState::Unused) 
				{
					a.deinitData();
					a.state = AtomState::Unused;
				}			
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
			size = sizeNext = 0u;
		}

		inline void reserve(std::size_t mCapacity) { if(getCapacity() < mCapacity) growCapacity(mCapacity); }

		template<typename... TArgs> inline Handle<T> create(TArgs&&... mArgs)
		{
			// `sizeNext` may be greater than the sizes of the vectors - resize vectors if needed 
			growIfNeeded();

			// `sizeNext` now is the first empty valid index - we create our atom there
			atoms[sizeNext].initData(std::forward<TArgs>(mArgs)...);
			atoms[sizeNext].state = AtomState::Alive;

			// Update the mark
			auto cIdx(atoms[sizeNext].markIdx);
			auto& mark(marks[cIdx]);
			mark.idx = sizeNext;
			++mark.ctr;

			// Update sizeNext free index
			++sizeNext;

			return {*this, cIdx, mark.ctr};	
		}	

		inline void refresh()
		{
			auto lastK(0u);
			Idx lastAlive{0u};
			
			// Find first alive and first dead atoms
			for(auto i(0u); i < sizeNext; ++i)
			{
				if(atoms[i].state == AtomState::Alive) lastAlive = i;
				else if(atoms[i].state == AtomState::Dead) 
				{
					lastK = i;
					break;
				}
			}	

			for(auto i(lastAlive + 1); i < sizeNext; ++i)
			{
				// Skip alive atoms
				if(atoms[i].state == AtomState::Alive) continue;

				// Found a dead atom - `i` now stores its index
				// Look for an alive atom after the dead atom
				for(auto k(lastK); true; ++k)
				{
					if(atoms[k].state == AtomState::Alive)
					{
						// Found an alive atom after dead `i` atom
						std::swap(atoms[i], atoms[k]);
						lastAlive = i;
						lastK = k;
						break;
					}

					// No more alive atoms, continue					
					if(k == sizeNext) goto later;					
				}
			}

			later:

			// [lastAlive + 1, sizeNext) contains only dead atoms, clean them up
			for(auto j(lastAlive + 1); j < sizeNext; ++j)				
			{
				atoms[j].deinitData();
				atoms[j].state = AtomState::Unused;
				++(getMarkFromAtom(atoms[j]).ctr);				
			}	

			// Starting from the beginning, update alive entities and their marks			
			auto n(0u);
			for(; n <= lastAlive; ++n) getMarkFromAtom(atoms[n]).idx = n;

			size = sizeNext = n; // Update size 		
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

		inline std::size_t getSize() const noexcept 	{ return size; }
		inline std::size_t getSizeNext() const noexcept { return sizeNext; }

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




void doTest()
{
	for(int j = 0; j < 2; ++j)
	{
		Manager<std::string> test;

		for(int k = 0; k < 2; ++k)
		{
			auto a0 = test.create();
			auto a1 = test.create();
			auto a2 = test.create();
			auto a3 = test.create();
			auto a4 = test.create();
			auto a5 = test.create();
			auto a6 = test.create();

			SSVU_ASSERT(test.getSize() == 0);
			SSVU_ASSERT(test.getSizeNext() == 7);

			test.refresh();

			SSVU_ASSERT(test.getSize() == 7);
			SSVU_ASSERT(test.getSizeNext() == 7);

			*a0 = "hi";
			a4.get() = "ciao";
			a6.get() = "bye";

			a2.destroy();
			a3.destroy();
			a5.destroy();

			SSVU_ASSERT(test.getSize() == 7);
			SSVU_ASSERT(test.getSizeNext() == 7);

			test.refresh();

//ssvu::lo("BUG") << test.getSize() << std::endl;
			SSVU_ASSERT(test.getSize() == 4);
			SSVU_ASSERT(test.getSizeNext() == 4);

			SSVU_ASSERT(*a0 == "hi");
			SSVU_ASSERT(*a4 == "ciao");
			SSVU_ASSERT(*a6 == "bye");

			SSVU_ASSERT(a0.isAlive());
			SSVU_ASSERT(a1.isAlive());
			SSVU_ASSERT(!a2.isAlive());
			SSVU_ASSERT(!a3.isAlive());
			SSVU_ASSERT(a4.isAlive());
			SSVU_ASSERT(!a5.isAlive());
			SSVU_ASSERT(a6.isAlive());

			test.forEach([](std::string& mStr){ mStr += "bb"; });

			SSVU_ASSERT(*a0 == "hibb");
			SSVU_ASSERT(*a4 == "ciaobb");
			SSVU_ASSERT(*a6 == "byebb");

			SSVU_ASSERT(a0.isAlive());
			SSVU_ASSERT(a1.isAlive());
			SSVU_ASSERT(!a2.isAlive());
			SSVU_ASSERT(!a3.isAlive());
			SSVU_ASSERT(a4.isAlive());
			SSVU_ASSERT(!a5.isAlive());
			SSVU_ASSERT(a6.isAlive());

			auto aNew = test.create();
			*aNew = "hehe";

			SSVU_ASSERT(test.getSize() == 4);
			SSVU_ASSERT(test.getSizeNext() == 5);

			test.refresh();

			SSVU_ASSERT(test.getSize() == 5);
			SSVU_ASSERT(test.getSizeNext() == 5);

			SSVU_ASSERT(a0.isAlive());
			SSVU_ASSERT(a1.isAlive());
			SSVU_ASSERT(!a2.isAlive());
			SSVU_ASSERT(!a3.isAlive());
			SSVU_ASSERT(a4.isAlive());
			SSVU_ASSERT(!a5.isAlive());
			SSVU_ASSERT(a6.isAlive());
			SSVU_ASSERT(aNew.isAlive());

			SSVU_ASSERT(*aNew == "hehe");
			test.clear();
		}		
	}	
}





volatile int state{0};

template<typename T> struct OV : public T { bool alive{true}; };

void doBench()
{
	using namespace ssvu;

	struct OSmall
	{
		char k[16];
		int myState;
		void a() { ++state; ++myState; }
	};
	struct OBig
	{
		char k[128];
		int myState;
		void a() { ++state; ++myState; }
	};

	constexpr std::size_t s(1000000);

	{
		{
			Benchmark::start("Vector - creation");
			std::vector<Uptr<OV<OSmall>>> storage;
			storage.reserve(s + 10000);
			Benchmark::endLo();

			Benchmark::start("Vector - filling");
			for(auto i(0u); i < s; ++i) storage.emplace_back(new OV<OSmall>);
			Benchmark::endLo();

			Benchmark::start("Vector - iteration");
			for(auto& o : storage) o->a();
			Benchmark::endLo();

			Benchmark::start("Vector - set dead");
			for(auto i(0u); i < s; i += 2) storage[i]->alive = false;
			Benchmark::endLo();

			Benchmark::start("Vector - refresh");
			eraseRemoveIf(storage, [](const Uptr<OV<OSmall>>& mO){ return !mO->alive; });
			Benchmark::endLo();

			Benchmark::start("Vector - add/rem");
			for(auto k(0u); k < 3; ++k)
			{
				for(auto j(0u); j < 10000; ++j) 
				{
					auto& u(getEmplaceUptr<OV<OSmall>>(storage));	
					u.alive = false;
				}

				eraseRemoveIf(storage, [](const Uptr<OV<OSmall>>& mO){ return !mO->alive; });
			}
			Benchmark::endLo();

			Benchmark::start("Vector - cleanup");
		}

		Benchmark::endLo();
	}

	ssvu::lo() << "" << std::endl;

	{
		{
			Benchmark::start("AtomM - creation");
			Manager<OSmall> storage;
			storage.reserve(s + 10000);
			Benchmark::endLo();

			Benchmark::start("AtomM - filling");
			for(auto i(0u); i < s; ++i) storage.create();
			Benchmark::endLo();

			Benchmark::start("AtomM - refresh");
			storage.refresh();
			Benchmark::endLo();

			Benchmark::start("AtomM - iteration");
			storage.forEach([](OSmall& mO){ mO.a(); });
			Benchmark::endLo();

			Benchmark::start("AtomM - set dead");
			storage.forEachAtom([](decltype(storage)::AtomType& mO){ mO.setDead(); });
			Benchmark::endLo();

			Benchmark::start("AtomM - refresh");
			storage.refresh();
			Benchmark::endLo();

			Benchmark::start("AtomM - add/rem");
			for(auto k(0u); k < 3; ++k)
			{
				for(auto j(0u); j < 10000; ++j) 
				{
					auto h(storage.create());
					h.destroy();					
				}

				storage.refresh();
			}
			Benchmark::endLo();

			Benchmark::start("AtomM - cleanup");
		}

		Benchmark::endLo();
	}
}

int main()
{
	doTest();
	doBench();
	return 0;
}