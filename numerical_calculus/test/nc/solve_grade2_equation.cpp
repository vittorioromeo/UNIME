#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

template <typename T0, typename T1, typename T2, typename T3, typename T4>
void test_equation(T0 a, T1 b, T2 c, T3 x0, T4 x1)
{
    auto results(nc::solve_grade2_equation(a, b, c));
    TEST_ASSERT_OP(std::get<0>(results) - x0, <, 0.05f);
    TEST_ASSERT_OP(std::get<1>(results) - x1, <, 0.05f);
}

int main()
{
    test_equation(4.f, 6.f, 2.f, // .
        -1.f, -0.5f);

    test_equation(4.f, 6.f, 0.f, // .
        -1.5f, -0.f);

    // Should not pass:
    // test_equation(4.f, 0.f, 2.f, // .
    //    -std::sqrt(0.5f), std::sqrt(0.5f));

    test_equation(4.f, 0.f, -2.f, // .
        -std::sqrt(0.5f), std::sqrt(0.5f));

    return 0;
}
