#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>

int main()
{
    auto m = nc::make_square_matrix<float, 3>( // .
        1, 2, 4,                               // .
        3, 8, 14,                              // .
        2, 6, 13);

    auto r = nc::make_ul_decomposition(m);

    for(std::size_t i(0); i < 3; ++i)
    {
        for(std::size_t j(0); j < 3; ++j)
        {
            //std::cout << "I:" << i << "\tJ: " << j << "\n";
            std::cout << r(i, j) << ", ";
        }

        std::cout << "\n";
    }

    std::cout << "\n";std::cout << "\n";
/*
    for(std::size_t i(0); i < 3; ++i)
    {
        for(std::size_t j(0); j < 3; ++j)
        {
            //std::cout << "I:" << i << "\tJ: " << j << "\n";
            std::cout << std::get<1>(r)(i, j) << ", ";
        }

        std::cout << "\n";
    }
*/
    return 0;
}
