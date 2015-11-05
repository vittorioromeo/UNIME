#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

int main()
{
    {
        auto r(nc::is_multiple_of(0, 1));
        TEST_ASSERT_OP(r, ==, true);
    }

    {
        auto r(nc::is_multiple_of(0, -1));
        TEST_ASSERT_OP(r, ==, true);
    }

    {
        auto r(nc::is_multiple_of(1, 1));
        TEST_ASSERT_OP(r, ==, true);
    }

    {
        auto r(nc::is_multiple_of(1, 2));
        TEST_ASSERT_OP(r, ==, false);
    }

    {
        auto r(nc::is_multiple_of(2, 1));
        TEST_ASSERT_OP(r, ==, true);
    }

    {
        auto r(nc::is_multiple_of(2, 2));
        TEST_ASSERT_OP(r, ==, true);
    }

    {
        auto r(nc::is_multiple_of(2, 4));
        TEST_ASSERT_OP(r, ==, false);
    }

    {
        auto r(nc::is_multiple_of(4, 2));
        TEST_ASSERT_OP(r, ==, true);
    }

    {
        auto r(nc::is_multiple_of(-4, 4));
        TEST_ASSERT_OP(r, ==, true);
    }

    {
        auto r(nc::is_multiple_of(-4, -4));
        TEST_ASSERT_OP(r, ==, true);
    }

    {
        TEST_ASSERT_OP(nc::is_even(0), ==, true);
        TEST_ASSERT_OP(nc::is_even(2), ==, true);
        TEST_ASSERT_OP(nc::is_even(-2), ==, true);
        TEST_ASSERT_OP(nc::is_even(4), ==, true);

        TEST_ASSERT_OP(nc::is_odd(0), ==, false);
        TEST_ASSERT_OP(nc::is_odd(2), ==, false);
        TEST_ASSERT_OP(nc::is_odd(-2), ==, false);
        TEST_ASSERT_OP(nc::is_odd(4), ==, false);

        TEST_ASSERT_OP(nc::is_odd(1), ==, true);
        TEST_ASSERT_OP(nc::is_odd(-1), ==, true);
        TEST_ASSERT_OP(nc::is_odd(3), ==, true);
        TEST_ASSERT_OP(nc::is_odd(-3), ==, true);
    }

    return 0;
}
