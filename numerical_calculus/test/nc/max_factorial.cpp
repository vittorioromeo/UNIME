#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

int main()
{
    TEST_ASSERT_OP(1932053504, ==, nc::max_representable_factorial<int>());

    TEST_ASSERT_OP(
        2.95232e+38 - nc::max_representable_factorial_real<float>(), <, 1.f);

    return 0;
}
