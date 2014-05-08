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

		private:
			Idx markIdx;
			bool alive{false};
			Uncertain<T> data;

			// Initializes the internal data
			template<typename... TArgs> inline void initData(TArgs&&... mArgs) 
				noexcept(noexcept(data.init(std::forward<TArgs>(mArgs)...)))
			{
				SSVU_ASSERT(!alive);
				data.init(std::forward<TArgs>(mArgs)...);
			}

			// Deinitializes the internal data
			inline void deinitData() noexcept(noexcept(data.deinit()))
			{ 
				SSVU_ASSERT(!alive);
				data.deinit();
			}

		public:
			inline Atom() = default;
			inline Atom(Atom&&) = default;
			inline Atom& operator=(Atom&&) = default;

			inline T& getData() noexcept 				{ SSVU_ASSERT(alive); return data.get(); }				
			inline const T& getData() const noexcept 	{ SSVU_ASSERT(alive); return data.get(); }	
			inline void setDead() noexcept 				{ alive = false; }
			
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
			refresh();
			for(auto i(0u); i < size; ++i) 				
			{
				SSVU_ASSERT(atoms[i].alive);
				atoms[i].alive = false;						
				atoms[i].deinitData();
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
			atoms[sizeNext].alive = true;

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
			// Type must be signed, to check with negative values later
			int iAlive{0}, iDead{0};
			
			// Find first alive and first dead atoms
			while(iDead < sizeNext && atoms[iDead].alive) ++iDead;			
			iAlive = iDead - 1;

			for(int i{iDead}; i < sizeNext; ++i)
			{
				// Skip alive atoms
				if(atoms[i].alive) continue;

				// Found a dead atom - `i` now stores its index
				// Look for an alive atom after the dead atom
				for(int k{iDead + 1}; true; ++k)
				{
					if(atoms[k].alive)
					{
						// Found an alive atom after dead `i` atom
						std::swap(atoms[i], atoms[k]);
						iAlive = i;
						iDead = k;
						break;
					}

					// No more alive atoms, continue					
					if(k == sizeNext) goto later;					
				}
			}

			later:

			// [iAlive + 1, sizeNext) contains only dead atoms, clean them up
			for(int j{iAlive + 1}; j < sizeNext; ++j)				
			{
				atoms[j].deinitData();
				++(getMarkFromAtom(atoms[j]).ctr);				
			}	

			// Starting from the beginning, update alive entities and their marks			
			int n{0};
			for(; n <= iAlive; ++n) getMarkFromAtom(atoms[n]).idx = n;

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
			for(const auto& a : atoms) std::cout << std::setw(4) << std::left << (int)a.alive << " ";
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


int ctorCalls, dtorCalls;

void doTest()
{
	struct OTest
	{ 
		std::string s;
		OTest() { ++ctorCalls; }
		~OTest() { ++dtorCalls; }
	}; 

	for(int j = 0; j < 2; ++j)
	{
		Manager<OTest> test;

		for(int k = 0; k < 2; ++k)
		{
			ctorCalls = dtorCalls = 0;

			auto a0 = test.create();
			auto a1 = test.create();
			auto a2 = test.create();
			auto a3 = test.create();
			auto a4 = test.create();
			auto a5 = test.create();
			auto a6 = test.create();

			SSVU_ASSERT(ctorCalls == 7);
			SSVU_ASSERT(dtorCalls == 0);
			SSVU_ASSERT(test.getSize() == 0);
			SSVU_ASSERT(test.getSizeNext() == 7);

			test.refresh();

			SSVU_ASSERT(ctorCalls == 7);
			SSVU_ASSERT(dtorCalls == 0);
			SSVU_ASSERT(test.getSize() == 7);
			SSVU_ASSERT(test.getSizeNext() == 7);

			a0->s = "hi";
			a4->s = "ciao";
			a6->s = "bye";

			a2.destroy();
			a3.destroy();
			a5.destroy();

			SSVU_ASSERT(ctorCalls == 7);
			SSVU_ASSERT(dtorCalls == 0);
			SSVU_ASSERT(test.getSize() == 7);
			SSVU_ASSERT(test.getSizeNext() == 7);

			test.refresh();

			SSVU_ASSERT(ctorCalls == 7);
			SSVU_ASSERT(dtorCalls == 3);
			SSVU_ASSERT(test.getSize() == 4);
			SSVU_ASSERT(test.getSizeNext() == 4);

			SSVU_ASSERT(a0->s == "hi");
			SSVU_ASSERT(a4->s == "ciao");
			SSVU_ASSERT(a6->s == "bye");

			SSVU_ASSERT(a0.isAlive());
			SSVU_ASSERT(a1.isAlive());
			SSVU_ASSERT(!a2.isAlive());
			SSVU_ASSERT(!a3.isAlive());
			SSVU_ASSERT(a4.isAlive());
			SSVU_ASSERT(!a5.isAlive());
			SSVU_ASSERT(a6.isAlive());

			test.forEach([](OTest& mA){ mA.s += "bb"; });

			SSVU_ASSERT(a0->s == "hibb");
			SSVU_ASSERT(a4->s == "ciaobb");
			SSVU_ASSERT(a6->s == "byebb");

			SSVU_ASSERT(a0.isAlive());
			SSVU_ASSERT(a1.isAlive());
			SSVU_ASSERT(!a2.isAlive());
			SSVU_ASSERT(!a3.isAlive());
			SSVU_ASSERT(a4.isAlive());
			SSVU_ASSERT(!a5.isAlive());
			SSVU_ASSERT(a6.isAlive());

			auto aNew = test.create();
			aNew->s = "hehe";

			SSVU_ASSERT(ctorCalls == 8);
			SSVU_ASSERT(dtorCalls == 3);
			SSVU_ASSERT(test.getSize() == 4);
			SSVU_ASSERT(test.getSizeNext() == 5);

			test.refresh();

			SSVU_ASSERT(ctorCalls == 8);
			SSVU_ASSERT(dtorCalls == 3);
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

			SSVU_ASSERT(aNew->s == "hehe");
			test.clear();

			SSVU_ASSERT(ctorCalls == 8);
			SSVU_ASSERT(dtorCalls == 8);
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