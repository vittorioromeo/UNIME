#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

int main()
{
    {
        std::vector<int> v{};
        std::vector<int> s{};
        TEST_ASSERT_NS_OP(nc::sort_vector(v), ==, s);
    }

    {
        std::vector<int> v{3, 2, 1};
        std::vector<int> s{1, 2, 3};
        TEST_ASSERT_NS_OP(nc::sort_vector(v), ==, s);
    }

    {
        std::vector<int> v{1, 2, 3};
        std::vector<int> s{1, 2, 3};
        TEST_ASSERT_NS_OP(nc::sort_vector(v), ==, s);
    }

    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7};
        TEST_ASSERT_NS_OP(nc::find_in_sorted_vector(3, v), ==, 2);
    }

    {
        std::vector<int> v{1, 2, 3};
        nc::insert_in_sorted_vector(2, v);
        std::vector<int> s{1, 2, 2, 3};
        TEST_ASSERT_NS_OP(v, ==, s);
    }

    {
        std::vector<int> v{1, 2, 3, 4};
        nc::insert_in_sorted_vector(3, v);
        std::vector<int> s{1, 2, 3, 3, 4};
        TEST_ASSERT_NS_OP(v, ==, s);
    }

    {
        std::vector<int> v{};
        nc::insert_in_sorted_vector(1, v);
        std::vector<int> s{1};
        TEST_ASSERT_NS_OP(v, ==, s);
    }

    {
        std::vector<int> v{1, 2, 3, 4};
        nc::remove_from_sorted_vector(2, v);
        std::vector<int> s{1, 2, 4};
        TEST_ASSERT_NS_OP(v, ==, s);
    }

    return 0;
}
