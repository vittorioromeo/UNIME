#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

int main()
{
    TEST_ASSERT_OP(
        std::numeric_limits<float>::epsilon() - nc::find_epsilon<float>(), <,
        1.f);

    TEST_ASSERT_OP(
        std::numeric_limits<double>::epsilon() - nc::find_epsilon<double>(), <,
        1.f);

    TEST_ASSERT_OP(std::numeric_limits<long double>::epsilon() -
                       nc::find_epsilon<long double>(),
        <, 1.f);

    return 0;
}
