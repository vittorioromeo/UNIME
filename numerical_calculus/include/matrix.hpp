#pragma once

#include <cstddef>
#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <limits>
#include <vrm/core/static_if.hpp>

#include <armadillo>
#include <eigen3/Eigen/Dense>

#include "multiples.hpp"
#include "equations.hpp"
#include "folds.hpp"
#include "representations.hpp"
#include "sorted_vector.hpp"

namespace nc
{
    struct init_identity
    {
    };

    struct dont_init
    {
    };

    template <typename T0, std::size_t TRowCount, std::size_t TColumnCount>
    class matrix
    {
    public:
        constexpr static auto row_count() noexcept { return TRowCount; }
        constexpr static auto column_count() noexcept { return TColumnCount; }

    private:
        std::array<T0, TRowCount * TColumnCount> _data;

        auto calc_index(std::size_t row, std::size_t column) const noexcept
        {
            assert(row < TRowCount);
            assert(column < TColumnCount);

            return column + TColumnCount * row;
        }

        auto calc_1d_index(std::size_t i) const noexcept
        {
            assert(i >= 0 && TColumnCount != 0);

            auto y(i / TColumnCount);
            return std::make_tuple(y, i - y * TColumnCount);
        }

    public:
        void clear()
        {
            for(std::size_t i(0); i < TRowCount; ++i)
                for(std::size_t j(0); j < TColumnCount; ++j)
                {
                    (*this)(i, j) = 0;
                }
        }

        void clear_to_identity()
        {
            static_assert(TRowCount == TColumnCount, "");

            clear();

            for(auto k(0); k < TRowCount; ++k)
            {
                (*this)(k, k) = 1;
            }
        }

        template <typename TF>
        void for_row_idxs(std::size_t i, TF&& f)
        {
            for(auto j(0); j < TColumnCount; ++j)
            {
                f(i, j);
            }
        }

        template <typename TF>
        void for_column_idxs(std::size_t j, TF&& f)
        {
            for(auto i(0); i < TRowCount; ++i)
            {
                f(i, j);
            }
        }

        // TODO: repetition also for_rows also for_columns
        template <typename TF>
        void for_idxs(TF&& f)
        {
            for(auto i(0); i < TRowCount; ++i)
                for(auto j(0); j < TColumnCount; ++j)
                {
                    f(i, j);
                }
        }

        template <typename TF>
        void for_idxs(TF&& f) const
        {
            for(auto i(0); i < TRowCount; ++i)
                for(auto j(0); j < TColumnCount; ++j)
                {
                    f(i, j);
                }
        }

        matrix(dont_init) {}
        matrix() { clear(); }
        matrix(init_identity) { clear_to_identity(); }

        matrix(const matrix& rhs) = default;
        matrix& operator=(const matrix& rhs) = default;

        matrix(matrix&& rhs) = default;
        matrix& operator=(matrix&& rhs) = default;

        // TODO: repetition
        template <typename TTpl>
        auto& at(const TTpl& t)
        {
            return (*this)(std::get<0>(t), std::get<1>(t));
        }

        template <typename TTpl>
        const auto& at(const TTpl& t) const
        {
            return (*this)(std::get<0>(t), std::get<1>(t));
        }

        // TODO: set_from_variadic -> set_from_vector
        template <typename... Ts>
        void set_from_vector(Ts&&... xs)
        {
            static_assert(sizeof...(xs) <= TRowCount * TColumnCount, "");

            std::vector<T0> vec{xs...};
            for(auto i(0u); i < vec.size(); ++i)
            {
                (*this).at(calc_1d_index(i)) = vec[i];
            }
        }

        // TODO: repetition
        auto& operator()(std::size_t row, std::size_t column)
        {
            return _data[calc_index(row, column)];
        }

        const auto& operator()(std::size_t row, std::size_t column) const
        {
            return _data[calc_index(row, column)];
        }

    private:
        template <typename T1, typename TF>
        auto simple_binary_op(
            const matrix<T1, TRowCount, TColumnCount>& rhs, TF&& f) const
        {
            using common = std::common_type_t<T0, T1>;
            matrix<common, TRowCount, TColumnCount> result{dont_init{}};

            for_idxs([this, &rhs, &result, &f](auto i, auto j)
                {
                    result(i, j) = f((*this)(i, j), rhs(i, j));
                });

            return result;
        }

    public:
        template <typename T1>
        auto operator+(const matrix<T1, TRowCount, TColumnCount>& rhs) const
        {
            return simple_binary_op(rhs, [](const auto& lv, const auto& rv)
                {
                    return lv + rv;
                });
        }

        template <typename T1>
        auto operator-(const matrix<T1, TRowCount, TColumnCount>& rhs) const
        {
            return simple_binary_op(rhs, [](const auto& lv, const auto& rv)
                {
                    return lv - rv;
                });
        }

        template <typename T1>
        auto operator==(const matrix<T1, TRowCount, TColumnCount>& rhs) const
        {
            for(auto i(0); i < TRowCount; ++i)
            {
                for(auto j(0); j < TColumnCount; ++j)
                {
                    if((*this)(i, j) != rhs(i, j))
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        template <typename T1>
        auto operator!=(const matrix<T1, TRowCount, TColumnCount>& rhs) const
        {
            return !(*this == rhs);
        }

        template <typename T1, std::size_t TRhsColumnCount>
        auto operator*(
            const matrix<T1, TColumnCount, TRhsColumnCount>& rhs) const
        {
            using common = std::common_type_t<T0, T1>;
            matrix<common, TRowCount, TRhsColumnCount> result{dont_init{}};

            for(std::size_t ri(0); ri < TRowCount; ++ri)
                for(std::size_t rj(0); rj < TRhsColumnCount; ++rj)
                {
                    common x(0);

                    for(auto k(0); k < TColumnCount; ++k)
                    {
                        x += ((*this)(ri, k) * rhs(k, rj));
                    }

                    result(ri, rj) = x;
                }

            return result;
        }

        // TODO: repetition with norm_inf
        auto norm_1() const noexcept
        {
            T0 curr_max(0);

            for(auto j(0); j < TColumnCount; ++j)
            {
                T0 temp_max(0);

                for(auto i(0); i < TRowCount; ++i)
                {
                    temp_max += abs((*this)(i, j));
                }

                curr_max = std::max(curr_max, temp_max);
            }

            return curr_max;
        }

        auto norm_inf() const noexcept
        {
            T0 curr_max(0);

            for(auto i(0); i < TRowCount; ++i)
            {
                T0 temp_max(0);

                for(auto j(0); j < TColumnCount; ++j)
                {
                    temp_max += abs((*this)(i, j));
                }

                curr_max = std::max(curr_max, temp_max);
            }

            return curr_max;
        }

    private:
        auto as_mapped_eigen() const noexcept
        {
            using eigen_matrix_type =
                Eigen::Matrix<T0, TRowCount, TColumnCount>;

            return Eigen::Map<const eigen_matrix_type>(
                _data.data(), TRowCount, TColumnCount);
        }

    public:
        auto norm_2() const noexcept
        {
            std::vector<double> v;
            for(const auto& x : _data) v.emplace_back(x);

            auto m = arma::mat::fixed<TRowCount, TColumnCount>(v.data());
            return arma::norm(m, 2);
            //  return as_mapped_eigen().template lpNorm<2>();
        }

        auto norm_frobenius() const noexcept
        {
            return as_mapped_eigen().norm();
        }

        constexpr auto static is_square() noexcept
        {
            return TRowCount == TColumnCount;
        }

        constexpr auto static order() noexcept
        {
            // static_assert(is_square(), "");
            return TColumnCount < TRowCount ? TColumnCount : TRowCount;
        }

        auto calc_minor(
            std::size_t row_to_skip, std::size_t column_to_skip) const noexcept
        {
            // static_assert(is_square(), "");

            auto result = matrix<T0, order() - 1, order() - 1>{dont_init{}};

            std::size_t target_column(0);
            std::size_t target_row(0);

            for(auto i(0); i < order(); ++i)
            {
                if(i == row_to_skip) continue;

                target_column = 0;
                for(auto j(0); j < order(); ++j)
                {
                    if(j == column_to_skip) continue;

                    result(target_row, target_column) = (*this)(i, j);
                    ++target_column;
                }

                ++target_row;
            }

            return result;
        }

        auto determinant() const noexcept
        {
            // static_assert(is_square(), "");


            return vrm::core::static_if(vrm::core::bool_v<(order() == 1)>)
                .then([](const auto& x)
                    {
                        return x(0, 0);
                    })
                .else_([](const auto& x)
                    {
                        T0 result(0);

                        for(int i = 0; i < order(); i++)
                        {
                            auto my_minor(x.calc_minor(0, i));

                            result += (i % 2 == 1 ? -1.0 : 1.0) * x(0, i) *
                                      my_minor.determinant();
                        }

                        return result;
                    })(*this);
        }

        auto inverse() const noexcept
        {
            auto result = matrix<T0, TRowCount, TColumnCount>{dont_init{}};

            vrm::core::static_if(vrm::core::bool_v<(order() == 1)>)
                .then([&result](const auto& x)
                    {
                        result(0, 0) = 1.0 / x(0, 0);
                    })
                .else_([&result](const auto& x)
                    {
                        auto det = 1.0 / x.determinant();

                        for(int j = 0; j < x.order(); j++)
                        {
                            for(int i = 0; i < x.order(); i++)
                            {
                                auto my_minor(x.calc_minor(j, i));
                                result(i, j) = det * my_minor.determinant();

                                if((i + j) % 2 == 1)
                                    result(i, j) = -result(i, j);
                            }
                        }

                    })(*this);

            return result;
        }

        auto perturbation_index() const noexcept
        {
            auto result(norm_2() * inverse().norm_2());
            assert(result >= 1);
            return result;
        }

        // TODO: refactor
        auto solve_gauss() const noexcept
        {
            static_assert(TColumnCount == TRowCount + 1, "");
            constexpr auto n(TRowCount);

            auto a = *this;

            for(int i = 0; i < n; i++)
            {
                // Search for maximum in this column
                double maxEl = abs(a(i, i));
                int maxRow = i;
                for(int k = i + 1; k < n; k++)
                {
                    if(abs(a(k, i)) > maxEl)
                    {
                        maxEl = abs(a(k, i));
                        maxRow = k;
                    }
                }

                // Swap maximum row with current row (column by column)
                for(int k = i; k < n + 1; k++)
                {
                    std::swap(a(maxRow, k), a(i, k));

                    /*
                    double tmp = a(maxRow, k);
                    a(maxRow, k) = a(i, k);
                    a(i, k) = tmp;
                    */
                }

                // Make all rows below this one 0 in current column
                for(int k = i + 1; k < n; k++)
                {
                    double c = -a(k, i) / a(i, i);
                    for(int j = i; j < n + 1; j++)
                    {
                        if(i == j)
                        {
                            a(k, j) = 0;
                        }
                        else
                        {
                            a(k, j) += c * a(i, j);
                        }
                    }
                }
            }

            // result
            matrix<T0, n, 1> x;

            // Solve equation Ax=b for an upper triangular matrix A
            for(int i = n - 1; i >= 0; i--)
            {
                x(i, 0) = a(i, n) / a(i, i);
                for(int k = i - 1; k >= 0; k--)
                {
                    a(k, n) -= a(k, i) * x(i, 0);
                }
            }

            return x;
        }

        auto solve_jacobi() const noexcept
        {
            double accuracy = 0.0001;

            static_assert(TColumnCount == TRowCount + 1, "");
            constexpr auto n(TRowCount);

            std::vector<double> newSolution(n, 0);
            std::vector<double> solution = newSolution;

            double maxDivergence = 0;


            do
            {
                maxDivergence = std::numeric_limits<double>::lowest();

                for(int row = 0; row < TRowCount; row++)
                {
                    auto x = (*this)(row, n);

                    for(int column = 0; column < n; column++)
                    {
                        if(row != column)
                        {
                            x -= (*this)(row, column) * solution[column];
                        }
                    }

                    x /= (*this)(row, row);

                    newSolution[row] = x;

                    double divergence =
                        std::fabs(solution[row] - newSolution[row]);

                    if(divergence > maxDivergence)
                    {
                        maxDivergence = divergence;
                    }
                }

                for(auto j : solution) std::cout << j << " ";
                std::cout << "\n divergence:  " << maxDivergence << "\n\n";
                solution = newSolution;
            } while(maxDivergence > accuracy);
            // TODO: verificare divergenza -> no soluzioni

            return solution;
        }

        template <typename TSolutions>
        auto solve_gauss_seidel(TSolutions phi)
        {
            double accuracy = 0.00000001;

            static_assert(TColumnCount == TRowCount + 1, "");
            constexpr auto n(TRowCount);

            // std::vector<double> phi(n, 0);
            auto old_phi = phi;
            double sigma;

            double maxDivergence = 0;

            do
            {

                maxDivergence = std::numeric_limits<double>::lowest();

                for(int i = 0; i < n; ++i)
                {
                    sigma = 0.0;

                    for(int j = 0; j < n; ++j)
                    {
                        if(i == j) continue;
                        sigma += (*this)(i, j) * phi[j];
                    }

                    phi[i] = (1.0 / ((*this)(i, i))) * ((*this)(i, n) - sigma);

                    double divergence = std::fabs(phi[i] - old_phi[i]);
                    if(divergence > maxDivergence)
                    {
                        maxDivergence = divergence;
                    }
                }

                old_phi = phi;

            } while(maxDivergence > accuracy);

            for(auto kk : phi) std::cout << kk << " | ";
            std::cout << "\n";
            return phi;
        }

        auto solve_gauss_seidel()
        {
            static_assert(TColumnCount == TRowCount + 1, "");
            constexpr auto n(TRowCount);

            std::vector<double> phi(n, 0);
            return solve_gauss_seidel(phi);
        }
    };
}
