#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <SSVUtils/SSVUtils.hpp>

template<typename T> struct InvPair
{
    using First = typename T::second_type;
    using Second = typename T::first_type;
    Second second;
    First first;
};

template<typename T> inline InvPair<T>& getInvPair(T& mPair) noexcept
{
  	return reinterpret_cast<InvPair<T>&>(mPair);
}

/*
template<typename T1, typename T2, typename T> class SneakyBimapTypeHelper;
template<typename T1, typename T2> class SneakyBimapTypeHelper<T1, T2, T1>
{
	using Current = T1;
	using Other = T2;
};
template<typename T1, typename T2> class SneakyBimapTypeHelper<T1, T2, T2>
{
	using Current = T2;
	using Other = T1;
};
*/

template<typename T1, typename T2> class SneakyBimap
{
	private:
		using PairType = std::pair<T1, T2>;
		using InvPairType = InvPair<PairType>;

		template<typename T> struct PtrComp
		{
  			inline bool operator()(const T* mA, const T* mB) const noexcept { return *mA < *mB; }
		};
		
		std::vector<ssvu::Uptr<PairType>> storage;
		std::set<const T1*, PtrComp<T1>> set1;
		std::set<const T2*, PtrComp<T2>> set2;

		inline const T2& get2From1(const T1* mItem) const noexcept
		{
			return reinterpret_cast<const PairType*>(mItem)->second;
		}
		inline const T1& get1From2(const T2* mItem) const noexcept
		{
			// FIX !?!?!?
			return reinterpret_cast<const InvPairType*>(mItem)->second;
		}

	public:
		inline SneakyBimap() = default;
		template<typename... TArgs> inline void emplace(TArgs&&... mArgs)
		{
			auto pair(new std::pair<T1, T2>{std::forward<TArgs>(mArgs)...});
			storage.emplace_back(pair);
			set1.emplace(&(pair->first));
			set2.emplace(&(pair->second));
		}
		inline void insert(const PairType& mPair)
		{

		}
		inline const T2& get(const T1& mKey) const noexcept
		{
			const auto& itr(set1.find(&mKey));
			return get2From1(*itr);
		}
		inline const T1& get(const T2& mKey) const noexcept
		{
			const auto& itr(set2.find(&mKey));
			return get1From2(*itr);
		}
};

int main()
{
	auto k = 10;
	SneakyBimap<int, std::string> sb;	
	sb.emplace(k, "banana");

	ssvu::lo() << sb.get(k) << std::endl;
	ssvu::lo() << sb.get("banana") << std::endl;

	return 0;
}
