#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>
#include <random>

template <int TV>
using int_v = std::integral_constant<int, TV>;

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

std::random_device rd;
std::mt19937 gen(rd());

auto rnd_float = [](auto min, auto max)
{
    return std::uniform_real_distribution<float>{min, max}(gen);
};

auto make_rnd_matrix_8 = []
{
    constexpr auto order(8);
    nc::matrix<float, order, order> m;
    m.for_idxs([&](auto xi, auto xj)
        {
            m(xi, xj) = rnd_float(-100.f, 100.f);
        });

    return m;
};


auto confront_norms = [](const auto& title, const auto& m)
{

    std::cout << title << "\n";
    print_matrix(m);
    std::cout << "\n";
    std::cout.flush();

    auto norm1 = m.norm_1();
    auto norm2 = m.norm_2();
    auto norminf = m.norm_inf();
    auto normfrob = m.norm_frobenius();

    std::cout << "Norm 1 of random matrix: " << norm1 << "\n";
    std::cout << "Norm 2 of random matrix: " << norm2 << "\n";
    std::cout << "Norm inf of random matrix: " << norminf << "\n";
    std::cout << "Norm (Frobenius) of random matrix: " << normfrob << "\n\n\n";


};


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


        for(auto i = 0; i < 10; ++i)
        {
            confront_norms(
                "Matrice casuale " + std::to_string(i), make_rnd_matrix_8());
        }
    }

    {
        nc::impl::for_args(
            [](auto x)
            {
                constexpr auto order = decltype(x){};
                auto m = nc::make_hilbert_matrix<float, order>();

                confront_norms("Hilbert, ordine " + std::to_string(order), m);
            },
            int_v<1>{}, int_v<2>{}, int_v<3>{}, int_v<4>{}, int_v<5>{},
            int_v<6>{});
    }

    {
        constexpr auto order = 8;
        auto verify_relationship_0 = [](const auto& m)
        {
            double coeff0(1.0 / std::sqrt((double)order));
            double x0(coeff0 * m.norm_inf());

            double x1(m.norm_2());

            double coeff2(std::sqrt((double)order));
            double x2(coeff2 * m.norm_inf());

            TEST_ASSERT_OP(x0, <=, x1);
            TEST_ASSERT_OP(x1, <=, x2);
        };

        auto verify_relationship_1 = [](const auto& m)
        {
            double coeff0(1.0 / std::sqrt((double)order));
            double x0(coeff0 * m.norm_1());

            double x1(m.norm_2());

            double coeff2(std::sqrt((double)order));
            double x2(coeff2 * m.norm_1());

            TEST_ASSERT_OP(x0, <=, x1);
            TEST_ASSERT_OP(x1, <=, x2);
        };

        auto verify_relationship_2 = [](const auto& m)
        {
            auto curr_max = m(0, 0);
            m.for_idxs([&curr_max, &m](auto i, auto j)
                {
                    curr_max = std::max(curr_max, m(i, j));
                });

            double x0(curr_max);

            double x1(m.norm_2());

            double coeff2((double)order);
            double x2(coeff2 * curr_max);

            TEST_ASSERT_OP(x0, <=, x1);
            TEST_ASSERT_OP(x1, <=, x2);
        };

        auto verify_relationship_3 = [](const auto& m)
        {
            double x0(m.norm_2());
            double x1(std::sqrt(m.norm_1() * m.norm_inf()));

            TEST_ASSERT_OP(x0, <=, x1);
        };

        for(auto i = 0; i < 10; ++i)
        {
            auto x = make_rnd_matrix_8();
            verify_relationship_0(x);
            verify_relationship_1(x);
            verify_relationship_2(x);
            verify_relationship_3(x);
        }
    }

    return 0;
}

// TODO: refactor
