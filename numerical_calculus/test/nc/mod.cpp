#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

int main()
{
    {
        auto r(nc::mod(10, 3));
        TEST_ASSERT_OP(r, ==, 1);
    }

    {
        auto r(nc::mod(97, 4));
        TEST_ASSERT_OP(r, ==, 1);
    }

    {
        auto r(nc::mod(2, 3));
        TEST_ASSERT_OP(r, ==, 2);
    }

    {
        auto r(nc::mod(-2, 3));
        TEST_ASSERT_OP(r, ==, 2);
    }

    {
        auto r(nc::mod(2, -3));
        TEST_ASSERT_OP(r, ==, 2);
    }

    {
        auto r(nc::mod(-2, -3));
        TEST_ASSERT_OP(r, ==, 2);
    }

    {
        auto r(nc::mod(0, -3));
        TEST_ASSERT_OP(r, ==, 0);
    }

    return 0;
}
