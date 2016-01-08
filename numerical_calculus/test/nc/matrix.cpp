#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

int main()
{
    {
        nc::matrix<int, 1, 1> m;
        TEST_ASSERT_OP(m(0, 0), ==, 0);
    }

    {
        auto m = nc::make_identity_matrix<int, 1>();
        TEST_ASSERT_OP(m(0, 0), ==, 1);
    }

    {
        auto m = nc::make_identity_matrix<int, 3>();
        TEST_ASSERT_OP(m(0, 0), ==, 1);
        TEST_ASSERT_OP(m(1, 1), ==, 1);
        TEST_ASSERT_OP(m(2, 2), ==, 1);

        TEST_ASSERT_OP(m(0, 1), ==, 0);
        TEST_ASSERT_OP(m(1, 0), ==, 0);
    }

    {
        auto m = nc::make_matrix<int, 2, 3>();

        TEST_ASSERT_OP(m(0, 0), ==, 0);
        TEST_ASSERT_OP(m(0, 1), ==, 0);
        TEST_ASSERT_OP(m(0, 2), ==, 0);
        TEST_ASSERT_OP(m(1, 0), ==, 0);
        TEST_ASSERT_OP(m(1, 1), ==, 0);
        TEST_ASSERT_OP(m(1, 2), ==, 0);
    }

    {
        auto m0 = nc::make_matrix<int, 2, 3>();
        auto m1 = nc::make_matrix<int, 2, 3>();

        m0(0, 0) = 5;
        m1(1, 1) = 3;

        m0(0, 1) = 2;
        m1(0, 1) = 2;

        auto m2 = m0 + m1;
        TEST_ASSERT_OP(m2(0, 0), ==, 5);
        TEST_ASSERT_OP(m2(0, 1), ==, 4);
        TEST_ASSERT_OP(m2(1, 1), ==, 3);
    }

    {
        auto m = nc::make_matrix<int, 2, 2>(0, 1, 2, 3);
        TEST_ASSERT_OP(m(0, 0), ==, 0);
        TEST_ASSERT_OP(m(0, 1), ==, 1);
        TEST_ASSERT_OP(m(1, 0), ==, 2);
        TEST_ASSERT_OP(m(1, 1), ==, 3);
    }

    {
        auto m1 = nc::make_matrix<int, 2, 1>(4, 5);
        auto m0 = nc::make_matrix<int, 1, 2>(2, 3);

        auto m2 = m0 * m1;
        TEST_ASSERT_OP(m2(0, 0), ==, 23);
    }

    {
        auto m0 = nc::make_matrix<int, 2, 2>(3, 4, 2, 3);
        auto m1 = nc::make_matrix<int, 2, 2>(5, 6, 5, 2);
        auto m2 = m0 * m1;

        TEST_ASSERT_OP(m2(0, 0), ==, 35);
        TEST_ASSERT_OP(m2(0, 1), ==, 26);
        TEST_ASSERT_OP(m2(1, 0), ==, 25);
        TEST_ASSERT_OP(m2(1, 1), ==, 18);
    }

    {
        auto v0 = nc::make_row_vector<int>(3, 4);
        auto v1 = nc::make_row_vector<int>(5, 6);

        auto s = nc::vector_scalar_product(v0, v1);
        TEST_ASSERT_OP(s(0, 0), ==, 39);

        auto t = nc::vector_tensor_product(v0, v1);
        TEST_ASSERT_OP(t(0, 0), ==, 15);
        TEST_ASSERT_OP(t(0, 1), ==, 18);
        TEST_ASSERT_OP(t(1, 0), ==, 20);
        TEST_ASSERT_OP(t(1, 1), ==, 24);
    }

    {
        auto m = nc::make_matrix<int, 1, 1>(3);

        TEST_ASSERT_OP(m.determinant(), ==, 3);
    }

    {
        auto m = nc::make_matrix<int, 2, 2>( // .
            3, 4,                            // .
            2, 3);

        TEST_ASSERT_OP(m.determinant(), ==, 1);
    }

    {
        auto m = nc::make_matrix<int, 3, 3>( // .
            1, 2, 3,                         // .
            2, 1, 5,                         //.
            4, 3, 1);

        TEST_ASSERT_OP(m.determinant(), ==, 28);
    }

    {
        auto x = nc::make_matrix<int, 1, 1>(1);
        auto m = x.inverse();

        TEST_ASSERT_OP(m(0, 0), ==, 1);
    }

    {
        auto x = nc::make_matrix<int, 2, 2>(3, 4, 2, 3);
        auto m = x.inverse();
        auto d = nc::make_matrix<int, 2, 2>(3, -4, -2, 3);

        TEST_ASSERT_NS_OP(m, ==, d);
    }

    return 0;
}
