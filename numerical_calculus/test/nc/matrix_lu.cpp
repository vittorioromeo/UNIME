#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

int main()
{
    auto a = nc::make_square_matrix<float, 3>( // .
        1, 2, 4,                               // .
        3, 8, 14,                              // .
        2, 6, 13);

    const auto& result(nc::make_crout_decomposition(a));

    const auto& l(std::get<0>(result));
    const auto& u(std::get<1>(result));

    /*
    auto print_matrix = [](const auto& x)
    {
        for(std::size_t i(0); i < x.row_count(); ++i)
        {
            for(std::size_t j(0); j < x.column_count(); ++j)
            {
                std::cout << x(i, j) << ", ";
            }

            std::cout << "\n";
        }
    };
    */

    TEST_ASSERT_NS_OP(l * u, ==, a);

    /*
    std::cout << "L:\n";
    print_matrix(l);
    std::cout << "\n\n\n";

    std::cout << "U:\n";
    print_matrix(u);
    std::cout << "\n\n\n";

    std::cout << "LU:\n";
    print_matrix(l * u);
    std::cout << "\n\n\n";
    */

    return 0;
}
