#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

int main()
{
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
        TEST_ASSERT_OP(nc::sum_n_numbers(v), ==, 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8);
    }

    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
        TEST_ASSERT_OP(
            nc::multiply_n_numbers(v), ==, 1 * 2 * 3 * 4 * 5 * 6 * 7 * 8);
    }

    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
        auto sum(1 + 2 + 3 + 4 + 5 + 6 + 7 + 8);
        auto expected(sum / v.size());
        TEST_ASSERT_OP(nc::average_n_numbers(v), ==, expected);
    }

    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
        TEST_ASSERT_OP(nc::min_n_numbers(v), ==, 1);
    }

    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
        TEST_ASSERT_OP(nc::max_n_numbers(v), ==, 8);
    }

    {
        // Should not run:
        // std::vector<int> v{};
        // TEST_ASSERT_OP(nc::max_n_numbers(v), ==, 8);
    }

    return 0;
}
