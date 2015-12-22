#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>
#include <random>

int main()
{
    {
        auto m = nc::make_matrix<int, 1, 1>(0);
        TEST_ASSERT_OP(m.norm_1(), ==, 0);
        TEST_ASSERT_OP(m.norm_2(), ==, 0);
        TEST_ASSERT_OP(m.norm_inf(), ==, 0);
    }

    {
        auto m = nc::make_matrix<int, 1, 1>(1);
        TEST_ASSERT_OP(m.norm_1(), ==, 1);
        TEST_ASSERT_OP(m.norm_2(), ==, 1);
        TEST_ASSERT_OP(m.norm_inf(), ==, 1);
    }

    {
        auto m = nc::make_matrix<float, 2, 2>( // .
            1, 1,                              // .
            2, 2);

        TEST_ASSERT_OP(m.norm_1(), ==, 3);

        // norm_2 is 3.16
        TEST_ASSERT_OP(m.norm_2(), >=, 3.1);
        TEST_ASSERT_OP(m.norm_2(), <=, 3.2);

        TEST_ASSERT_OP(m.norm_inf(), ==, 4);
    }

    {
        auto m = nc::make_vandermonde_matrix<float, 5>(
            nc::make_vector<float>(1, 2, 3, 4, 5));

        std::cout << "Norm 1 of vandermonde: " << m.norm_1() << "\n";
        std::cout << "Norm 2 of vandermonde: " << m.norm_2() << "\n";
        std::cout << "Norm inf of vandermonde: " << m.norm_inf() << "\n\n";
    }

    {
        auto m = nc::make_diagonal_dominant_square_matrix(1, 2, 4, 5, 6, 3);

        std::cout << "Norm 1 of dd square matrix: " << m.norm_1() << "\n";
        std::cout << "Norm 2 of dd square matrix: " << m.norm_2() << "\n";
        std::cout << "Norm inf of dd square matrix: " << m.norm_inf() << "\n\n";
    }

    {
        std::random_device rd;
        std::mt19937 gen(rd());

        auto rnd_float = [&](auto min, auto max)
        {
            return std::uniform_real_distribution<float>{min, max}(gen);
        };

        auto make_rnd_matrix = [&]
        {
            constexpr auto order(8);
            nc::matrix<float, order, order> m;
            m.for_idxs([&](auto xi, auto xj)
                {
                    m(xi, xj) = rnd_float(-100.f, 100.f);
                });

            return m;
        };

        auto print_matrix = [](const auto& x)
        {
            std::cout << "matrix([";
            for(std::size_t i(0); i < x.row_count(); ++i)
            {
                std::cout << "[";

                for(std::size_t j(0); j < x.column_count(); ++j)
                {
                    std::cout << x(i, j) << ", ";
                }

                std::cout << "],\n";
            }
            std::cout << "])";
        };

        auto confront_norms = [&]
        {
            auto m = make_rnd_matrix();

            auto norm1 = m.norm_1();
            auto norm2 = m.norm_2();
            auto norminf = m.norm_inf();
            auto normfrob = m.norm_frobenius();

            std::cout << "Random matrix:\n";
            print_matrix(m);
            std::cout << "\n";

            std::cout << "Norm 1 of random matrix: " << norm1 << "\n";
            std::cout << "Norm 2 of random matrix: " << norm2 << "\n";
            std::cout << "Norm inf of random matrix: " << norminf << "\n";
            std::cout << "Norm (Frobenius) of random matrix: " << normfrob
                      << "\n\n\n";


        };

        for(auto i = 0; i < 10; ++i)
        {
            confront_norms();
        }
    }
    return 0;
}
