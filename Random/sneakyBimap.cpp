#include <iostream>
#include <set>
#include <utility>
#include <cstddef>
#include <SSVUtils/SSVUtils.hpp>

namespace Internal
{
    template<typename T> struct PtrComparator
    {
        inline bool operator()(const T* mA, const T* mB) const noexcept { return *mA < *mB; }
    };

    /*
    template<typename T> struct InvPair
    {
        using Second = typename T::first_type;
        using First = typename T::second_type;
        Second second;
        First first;
    };

    template<typename T> inline InvPair<T>& getInvPair(T& mPair) noexcept { return reinterpret_cast<InvPair<T>&>(mPair); }
    */
}   

template<typename T1, typename T2> class SneakyBimap
{
    public:
        using BMPair = std::pair<T1, T2>;
        using Storage = std::vector<ssvu::Uptr<BMPair>>;
        template<typename T> using PtrSet = std::set<const T*, Internal::PtrComparator<T>>;

        using iterator = typename Storage::iterator;
        using const_iterator = typename Storage::const_iterator;
        using reverse_iterator = typename Storage::reverse_iterator;
        using const_reverse_iterator = typename Storage::const_reverse_iterator;

    private:           
        Storage storage;
        PtrSet<T1> set1;
        PtrSet<T2> set2;

        template<typename T> inline BMPair& getPairImpl(const T* mPtr) const noexcept
        {
            static_assert(std::is_standard_layout<BMPair>::value, "BMPair must have standard layout");
            return *(const_cast<BMPair*>(reinterpret_cast<const BMPair*>(mPtr)));
        }
        
        inline const char* getPairBasePtr(const T2* mItem) const noexcept
        {
            return reinterpret_cast<const char*>(mItem) - offsetof(BMPair, second);            
        }
        
        inline BMPair& getPair(const T1* mItem) const noexcept   { return getPairImpl(mItem); }
        inline BMPair& getPair(const T2* mItem) const noexcept   { return getPairImpl(getPairBasePtr(mItem)); }
        
        inline T2& getItem(const T1* mItem) noexcept    { return getPair(mItem).second; }
        inline T1& getItem(const T2* mItem) noexcept    { return getPair(mItem).first; }

        inline const T2& getItem(const T1* mItem) const noexcept    { return getPair(mItem).second; }
        inline const T1& getItem(const T2* mItem) const noexcept    { return getPair(mItem).first; }

    public:        
        template<typename TA1, typename TA2> inline void emplace(TA1&& mArg1, TA2&& mArg2)
        {
            assert(!this->has(mArg1) && !this->has(mArg2));

            auto pair(std::make_unique<std::pair<T1, T2>>(std::forward<TA1>(mArg1), std::forward<TA2>(mArg2)));
            set1.emplace(&(pair->first));
            set2.emplace(&(pair->second));
            storage.emplace_back(std::move(pair));
        }
        inline void insert(const BMPair& mPair)
        {
            this->emplace(mPair.first, mPair.second);
        }

        template<typename T> inline void erase(const T& mKey) 
        {
            assert(this->has(mKey)); 

            const auto& pairPtr(getPair(&mKey));
            
            set1.erase(&(pairPtr->first));
            set2.erase(&(pairPtr->second));

            ssvu::eraseRemoveIf(storage, [&pairPtr](const ssvu::Uptr<std::pair<T1, T2>>& mI){ return mI.get() == pairPtr; });

            assert(!this->has(mKey)); 
        }

        inline const T2& at(const T1& mKey) const noexcept
        {
            const auto& itr(set1.find(&mKey));
            if(itr == std::end(set1)) throw std::out_of_range{"mKey was not found in set1"};
            return getItem(*itr);
        }
        inline const T1& at(const T2& mKey) const noexcept
        {
            const auto& itr(set2.find(&mKey));
            if(itr == std::end(set2)) throw std::out_of_range{"mKey was not found in set2"};
            return getItem(*itr);
        }
        
        inline T2& operator[](const T1& mKey) noexcept  { assert(this->has(mKey)); return getItem(*set1.find(&mKey)); }
        inline T1& operator[](const T2& mKey) noexcept  { assert(this->has(mKey)); return getItem(*set2.find(&mKey)); }

        inline const T2& operator[](const T1& mKey) const noexcept  { assert(this->has(mKey)); return getItem(*set1.find(&mKey)); }
        inline const T1& operator[](const T2& mKey) const noexcept  { assert(this->has(mKey)); return getItem(*set2.find(&mKey)); }

        inline void clear() noexcept { storage.clear(); set1.clear(); set2.clear(); }

        inline bool empty() const noexcept { return storage.empty(); }
        inline auto size() const noexcept -> decltype(storage.size()) { return storage.size(); }

        inline auto count(const T1& mKey) const noexcept -> decltype(set1.count(&mKey)) { return set1.count(&mKey); } 
        inline auto count(const T2& mKey) const noexcept -> decltype(set2.count(&mKey)) { return set2.count(&mKey); }

        inline auto find(const T1& mKey) const noexcept -> decltype(set1.find(&mKey)) { return set1.find(&mKey); } 
        inline auto find(const T2& mKey) const noexcept -> decltype(set2.find(&mKey)) { return set2.find(&mKey); }

        inline bool has(const T1& mKey) const noexcept { return this->find(mKey) != std::end(set1); }
        inline bool has(const T2& mKey) const noexcept { return this->find(mKey) != std::end(set2); }

        inline auto begin()     noexcept        -> decltype(storage.begin())    { return storage.begin(); }
        inline auto end()       noexcept        -> decltype(storage.end())      { return storage.end(); }
        inline auto begin()     const noexcept  -> decltype(storage.begin())    { return storage.begin(); }
        inline auto end()       const noexcept  -> decltype(storage.end())      { return storage.end(); }
        inline auto cbegin()    const noexcept  -> decltype(storage.cbegin())   { return storage.cbegin(); }
        inline auto cend()      const noexcept  -> decltype(storage.cend())     { return storage.cend(); }
        inline auto rbegin()    noexcept        -> decltype(storage.rbegin())   { return storage.rbegin(); }
        inline auto rend()      noexcept        -> decltype(storage.rend())     { return storage.rend(); }
        inline auto crbegin()   const noexcept  -> decltype(storage.crbegin())  { return storage.crbegin(); }
        inline auto crend()     const noexcept  -> decltype(storage.crend())    { return storage.crend(); }
};

SSVU_TEST(SneakyBimapTests)
{
    SneakyBimap<int, std::string> sb;

    SSVUT_EXPECT(sb.empty());
    SSVUT_EXPECT(!sb.has(10));
    SSVUT_EXPECT(!sb.has("banana"));
    SSVUT_EXPECT(sb.count(10) == 0);
    SSVUT_EXPECT(sb.count("banana") == 0);

    sb.emplace(10, "banana");

    SSVUT_EXPECT(!sb.empty());
    SSVUT_EXPECT(sb.size() == 1);
    SSVUT_EXPECT(sb.has(10));
    SSVUT_EXPECT(sb.has("banana"));
    SSVUT_EXPECT(sb.count(10) == 1);
    SSVUT_EXPECT(sb.count("banana") == 1);
    SSVUT_EXPECT(sb.at(10) == "banana");
    SSVUT_EXPECT(sb[10] == "banana");
    SSVUT_EXPECT(sb.at("banana") == 10);
    SSVUT_EXPECT(sb["banana"] == 10);

    sb["banana"] = 25;

    SSVUT_EXPECT(!sb.empty());
    SSVUT_EXPECT(sb.size() == 1);
    SSVUT_EXPECT(!sb.has(10));
    SSVUT_EXPECT(sb.has(25));
    SSVUT_EXPECT(sb.has("banana"));
    SSVUT_EXPECT(sb.count(10) == 0);
    SSVUT_EXPECT(sb.count(25) == 1);
    SSVUT_EXPECT(sb.count("banana") == 1);
    SSVUT_EXPECT(sb.at(25) == "banana");
    SSVUT_EXPECT(sb[25] == "banana");
    SSVUT_EXPECT(sb.at("banana") == 25);
    SSVUT_EXPECT(sb["banana"] == 25);

    sb["banana"] = 15;
    sb[15] = "melon";
    sb.emplace(10, "cucumber");

    SSVUT_EXPECT(!sb.empty());
    SSVUT_EXPECT(sb.size() == 2);
    SSVUT_EXPECT(sb.has(10));
    SSVUT_EXPECT(sb.has(15));
    SSVUT_EXPECT(!sb.has(25));
    SSVUT_EXPECT(sb.has("melon"));
    SSVUT_EXPECT(sb.has("cucumber"));
    SSVUT_EXPECT(!sb.has("banana"));
    SSVUT_EXPECT(sb.count(10) == 1);
    SSVUT_EXPECT(sb.count(15) == 1);
    SSVUT_EXPECT(sb.count(25) == 0);
    SSVUT_EXPECT(sb.count("melon") == 1);
    SSVUT_EXPECT(sb.count("cucumber") == 1);
    SSVUT_EXPECT(sb.count("banana") == 0);
    SSVUT_EXPECT(sb.at(10) == "cucumber");
    SSVUT_EXPECT(sb[10] == "cucumber");
    SSVUT_EXPECT(sb.at("cucumber") == 10);
    SSVUT_EXPECT(sb["cucumber"] == 10);
    SSVUT_EXPECT(sb.at(15) == "melon");
    SSVUT_EXPECT(sb[15] == "melon");
    SSVUT_EXPECT(sb.at("melon") == 15);
    SSVUT_EXPECT(sb["melon"] == 15);

    sb.clear();

    SSVUT_EXPECT(sb.empty());
    SSVUT_EXPECT(sb.size() == 0);
    SSVUT_EXPECT(!sb.has(10));
    SSVUT_EXPECT(!sb.has(15));
    SSVUT_EXPECT(!sb.has(25));
    SSVUT_EXPECT(!sb.has("melon"));
    SSVUT_EXPECT(!sb.has("cucumber"));
    SSVUT_EXPECT(!sb.has("banana"));
}
SSVU_TEST_END();

int main()
{
    SSVU_TEST_RUN_ALL();
    return 0;
}
