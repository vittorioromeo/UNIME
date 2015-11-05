#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

template <typename T0>
auto find_epsilon()
{
    int i(2);

    while(true)
    {
        T0 curr(T0(1) / T0(10 * i));
        ++i;


        if(T0(T0(1) + T0(curr)) == T0(1))
        {
            std::cout << curr * 2 << "\n";


            break;
        }
    }
}

int main()
{
    std::cout << std::numeric_limits<float>::epsilon()  << std::endl;
    find_epsilon<float>();
    ;

    TEST_ASSERT_OP(1932053504, ==, nc::max_representable_factoral<int>());

    TEST_ASSERT_OP(
        2.95232e+38 - nc::max_representable_factoral_real<float>(), <, 1.f);



    return 0;
}
