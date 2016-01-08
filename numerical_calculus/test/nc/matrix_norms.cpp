#include "../utils/test_utils.hpp"
#include <nc_cpp.hpp>
#include <random>

template <int TV>
using int_v = std::integral_constant<int, TV>;

auto print_matrix = [](const auto& x)
{
    std::cout << "matrix(\n[\n";
    for(std::size_t i(0); i < x.row_count(); ++i)
    {
        std::cout << "[";

        for(std::size_t j(0); j < x.column_count(); ++j)
        {
            std::cout << x(i, j) << ", ";
        }

        std::cout << "],\n";
    }
    std::cout << "\n]\n)";
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

auto make_rnd_matrix_3 = []
{
    constexpr auto order(3);
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
    auto pindex = m.perturbation_index();

    std::cout << "Norm 1 of " << title << ": " << norm1 << "\n";
    std::cout << "Norm 2 of " << title << ": " << norm2 << "\n";
    std::cout << "Norm inf of " << title << ": " << norminf << "\n";
    std::cout << "Norm (Frobenius) of " << title << ": " << normfrob << "\n";
    std::cout << "Perturbation index of " << title << ": " << pindex
              << "\n\n\n";
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
            nc::make_row_vector<float>(1, 2, 3, 4, 5));

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
        nc::for_args(
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
            m.for_idxs([&curr_max, &m](auto xi, auto xj)
                {
                    curr_max = std::max(curr_max, m(xi, xj));
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

    // TODO: vettori equidistanti
    {
        nc::for_args(
            [](auto x)
            {
                using precision = float;

                constexpr auto component_count(decltype(x){});
                nc::row_vector<precision, component_count> v;

                constexpr precision interval_left(-1);
                constexpr precision interval_right(1);

                constexpr precision interval_range(
                    -(interval_left - interval_right));

                constexpr auto max_step(interval_range / (component_count - 1));

                for(auto j(0); j < component_count; ++j)
                {
                    nc::access_row_vector(v, j) = interval_left + max_step * j;
                }

                std::cout << "Norms of:\n";
                print_matrix(v);
                std::cout << "\n"
                          << "\tNorm   1: " << v.norm_1() << "\n"
                          << "\tNorm   2: " << v.norm_2() << "\n"
                          << "\tNorm inf: " << v.norm_inf() << "\n\n\n";
            },
            int_v<2>{}, int_v<3>{}, int_v<4>{}, int_v<5>{}, int_v<6>{},
            int_v<7>{}, int_v<8>{}, int_v<9>{}, int_v<10>{}, int_v<11>{},
            int_v<12>{}, int_v<13>{}, int_v<14>{}, int_v<15>{});
    }

    // perturbazioni
    {
        auto m = nc::make_matrix<float, 3, 3>( // .
            4, -1, 2,                          // .
            1, 3, 1,                           // .
            0, -3, 5);

        auto p_index = m.perturbation_index();
        // = ~2.42486

        TEST_ASSERT_OP(p_index, >, 2.4);
        TEST_ASSERT_OP(p_index, <, 2.45);
    }

    // jacobi test
    {
        auto precision_test = [](auto a, auto b, auto prec)
        {
            return std::abs(a - b) < prec;
        };

        // gauss seidel test (0.81, -0.66)
        {
            auto m_gs = nc::make_matrix<float, 2, 3>( // .
                16, 3, 11,                            // .
                7, -11, 13                            // .
                );

            auto rgs = m_gs.solve_gauss_seidel();
            std::cout << "GAUSS_SEIDEL ^ \n\n";

            auto xj0 = nc::access_column_vector(rgs, 0);
            auto xj1 = nc::access_column_vector(rgs, 1);

            TEST_ASSERT(precision_test(xj0, 0.81, 0.5f));
            TEST_ASSERT(precision_test(xj1, -0.66, 0.5f));
        }

        // jacobi test (1, 2, -1)
        {
            auto jm = nc::make_matrix<float, 3, 4>( // .
                4, -1, -1, 3,                       // .
                -2, 6, 1, 9,                        // .
                -1, 1, 7, -6);

            auto rj = jm.solve_jacobi();

            auto xj0 = nc::access_column_vector(rj, 0);
            auto xj1 = nc::access_column_vector(rj, 1);
            auto xj2 = nc::access_column_vector(rj, 2);

            TEST_ASSERT(precision_test(xj0, 1.f, 0.5f));
            TEST_ASSERT(precision_test(xj1, 2.f, 0.5f));
            TEST_ASSERT(precision_test(xj2, -1.f, 0.5f));
        }
    }

    // gauss
    {
        auto float_test = [](auto a, auto b)
        {
            return std::abs(a - b) < 0.001;
        };

        auto m = nc::make_matrix<float, 3, 4>( // .
            1, 1, 1, 0,                        // .
            1, -2, 2, 4,                       // .
            1, 2, -1, 2);
        {
            auto r = m.solve_gauss();

            auto x0 = nc::access_column_vector(r, 0);
            auto x1 = nc::access_column_vector(r, 1);
            auto x2 = nc::access_column_vector(r, 2);

            TEST_ASSERT(float_test(x0, 4.f));
            TEST_ASSERT(float_test(x1, -2.f));
            TEST_ASSERT(float_test(x2, -2.f));
        }

        // perturbazione
        m(1, 1) += 0.001;

        {
            auto r = m.solve_gauss();
            auto x0 = nc::access_column_vector(r, 0);
            auto x1 = nc::access_column_vector(r, 1);
            auto x2 = nc::access_column_vector(r, 2);

            std::cout << "solutions: " << x0 << ", " << x1 << ", " << x2
                      << "\n";

            // non mal condizionata
            /*
                        TEST_ASSERT(float_test(x0, 4.f));
                        TEST_ASSERT(float_test(x1, -2.f));
                        TEST_ASSERT(float_test(x2, -2.f));*/
        }


        //
    }

    // teorema wilk
    {
        for(int i = 0; i < 5; ++i)
        {
            auto n = 3;
            auto m = make_rnd_matrix_3();
            auto lu = nc::make_crout_decomposition(m);
            const auto& u(std::get<1>(lu));
            auto u_nn(std::abs(u(n - 1, n - 1)));

            float max_m = 0;
            m.for_idxs([&](auto xi, auto xj)
                {
                    max_m = std::max(max_m, std::abs(m(xi, xj)));
                });

            auto coeff = std::pow(2.f, n - 1);
            auto rhs = coeff * max_m;

            TEST_ASSERT_OP(u_nn, <=, rhs);
        }
    }


    // ese matrici wilk
    {
        auto m = nc::make_wilkinson_p_matrix<float, 7>();
        print_matrix(m);
    }

    {
        auto m = nc::make_wilkinson_m_matrix<float, 7>();
        print_matrix(m);
    }

    // ese parte IV n 4
    {
        auto m = nc::make_matrix<float, 2, 3>( // .
            0.96326, 0.81321, 0.88824,         // .
            0.81321, 0.68654, 0.74988          // .
            );

        auto our_phi = nc::make_column_vector<float>(0.33116, 0.7);

        auto res = m.solve_gauss_seidel(our_phi);
        (void)res;

        std::cout << "\n\n\n";
        m.solve_gauss_seidel();

        std::cout << "cond indx: " << m.perturbation_index() << "\n";
    }

    // lagrange test
    if(false)
    {
        auto f = [](auto val)
        {
            return 1.0 / val;
        };

        nc::column_vector<float, 3> x, fx;


        auto add = [&](auto i, auto value)
        {
            nc::access_column_vector(x, i) = value;
            nc::access_column_vector(fx, i) = f(value);
        };

        add(0, 2.0);
        add(1, 2.5);
        add(2, 4.0);

        {
            auto li = nc::lagrange_interpolator(x, fx);

            auto f2 = [](auto value)
            {
                return 0.05 * std::pow(value, 2) - 0.425 * value + 1.15;
            };

            for(int i = 0; i < 20; ++i)
            {
                std::cout << "r: " << li(i) << " == " << f2(i) << " == " << f(i)
                          << "\n";
            }
        }

        std::cout << "newton xd: \n\n";

        {
            auto li = nc::newton_interpolator(x, fx);

            auto f2 = [](auto value)
            {
                return 0.05 * std::pow(value, 2) - 0.425 * value + 1.15;
            };

            for(int i = 0; i < 20; ++i)
            {
                std::cout << "r: " << li(i) << " == " << f2(i) << " == " << f(i)
                          << "\n";
            }
        }
    }

    {
        auto f = [](auto value)
        {
            return value + 10;
        };

        nc::column_vector<float, 5> x, fx;


        auto add = [&](auto i, auto value, auto value2)
        {
            nc::access_column_vector(x, i) = value;
            nc::access_column_vector(fx, i) = value2;
        };

        add(0, -3.0, 10.0);
        add(1, -2.0, 2.0);
        add(2, 1.0, 3.0);
        add(3, 3.0, 12.0);
        add(4, 2.0, 10.0);

        if(false)
        {
            auto li = nc::lagrange_interpolator(x, fx);

            for(int i = 0; i < 20; ++i)
            {
                std::cout << "r: " << li(i) << " == " << f(i) << "\n";
            }
        }

        std::cout << "newton xd: \n\n";

        {
            auto li = nc::newton_interpolator(x, fx);

            auto f2 = [](auto v)
            {
                auto c1 = (-1.0 / 8.0) * std::pow(v, 4);
                auto c2 = (-1.0 / 3.0) * std::pow(v, 3);
                auto c3 = (21.0 / 8.0) * std::pow(v, 2);
                auto c4 = (10.0 / 3.0) * v;
                auto c5 = -5.0 / 2.0;
                /*
                #define PRINT(x) std::cout << #x << " : " << x << "\n";

                                        PRINT(c1)
                                        PRINT(c2)
                                        PRINT(c3)
                                        PRINT(c4)
                                        PRINT(c5)
                */
                return c1 + c2 + c3 + c4 + c5;

                // return (1.0 / 3.0) *
                //    (2 * std::pow(v, 3) - 9 * std::pow(v, 2) + 10 * v);
            };

            for(int i = 0; i < 20; ++i)
            {
                std::cout << "r: " << li(i) << " == " << f2(i) << "\n";
            }
        }
    }

    {
        std::cout << "creating x\n";
        auto x = nc::make_column_vector<float>(-1, 1, 2);

        std::cout << "creating fx\n";
        auto fx = nc::make_column_vector<float>(2, 1, 1);

        std::cout << "creating mi\n";
        auto mi = nc::monomial_interpolator<3>(x, fx);

        // print_matrix(mi);
    }

    return 0;
}

// TODO: refactor
