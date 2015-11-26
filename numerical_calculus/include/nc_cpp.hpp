#pragma once

#include <cstddef>
#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <limits>

namespace nc
{
    template <typename T0>
    auto abs(T0 x)
    {
        return x < 0 ? -x : x;
    }

    template <typename T0, typename T1>
    auto mod(T0 a, T1 b)
    {
        assert(b != 0);

        a = abs(a);
        b = abs(b);

        auto division_result(a / b);
        auto multiplication_result(division_result * b);
        return a - multiplication_result;
    }

    template <typename T0, typename T1>
    auto is_multiple_of(T0 a, T1 b)
    {
        return mod(a, b) == 0;
    }

    template <typename T0>
    auto is_even(T0 x)
    {
        return mod(x, 2) == 0;
    }

    template <typename T0>
    auto is_odd(T0 x)
    {
        return mod(x, 2) != 0;
    }

    template <typename T0, typename T1, typename T2>
    auto delta(T0 a, T1 b, T2 c)
    {
        auto x(std::pow(b, 2) - (4 * a * c));
        assert(x >= 0);

        return std::sqrt(x);
    }

    template <typename T0, typename T1, typename T2>
    auto solve_grade2_equation(T0 a, T1 b, T2 c)
    {
        assert(a != 0);

        auto my_delta(delta(a, b, c));
        auto solution([&my_delta, &a, &b](auto sign)
            {
                return (-b - (my_delta * sign)) / (2 * a);
            });

        auto x0(solution(1));
        auto x1(solution(-1));

        return std::make_tuple(x0, x1);
    }

    template <typename T0, typename TF>
    auto fold(TF&& f, T0 initial_value, const std::vector<T0>& vec)
    {
        T0 result(initial_value);
        for(const auto& x : vec) f(result, x);
        return result;
    }

    template <typename T0>
    auto sum_n_numbers(const std::vector<T0>& vec)
    {
        return fold(
            [](auto& r, const auto& v)
            {
                r += v;
            },
            0, vec);
    }

    template <typename T0>
    auto multiply_n_numbers(const std::vector<T0>& vec)
    {
        return fold(
            [](auto& r, const auto& v)
            {
                r *= v;
            },
            1, vec);
    }

    template <typename T0>
    auto average_n_numbers(const std::vector<T0>& vec)
    {
        return sum_n_numbers(vec) / vec.size();
    }

    template <typename T0>
    auto min_n_numbers(const std::vector<T0>& vec)
    {
        assert(vec.size() > 0);

        return fold(
            [](auto& r, const auto& v)
            {
                r = std::min(r, v);
            },
            vec[0], vec);
    }

    template <typename T0>
    auto max_n_numbers(const std::vector<T0>& vec)
    {
        assert(vec.size() > 0);

        return fold(
            [](auto& r, const auto& v)
            {
                r = std::max(r, v);
            },
            vec[0], vec);
    }

    namespace impl
    {
        template <typename T0, typename TF>
        auto max_representable_factoral(TF&& f)
        {
            T0 last_n(1), n(1);

            for(int i(2); f(last_n, n); ++i)
            {
                last_n = n;
                n *= i;
            }

            return last_n;
        }
    }

    template <typename T0>
    auto max_representable_factoral()
    {
        return impl::max_representable_factoral<T0>(
            [](const auto& last_n, const auto& n)
            {
                return last_n <= n;
            });
    }

    template <typename T0>
    auto max_representable_factoral_real()
    {
        return impl::max_representable_factoral<T0>(
            [](const auto&, const auto& n)
            {
                return !std::isnan(n) && !std::isinf(n);
            });
    }

    template <typename T0>
    auto find_epsilon()
    {
        T0 curr(0);

        for(int i(2); T0(1) + curr != T0(1); ++i)
        {
            curr = T0(1) / T0(10 * i);
        }

        return curr * 2;
    }

    namespace impl
    {
        template <typename T0>
        void sort_vector(T0* a, int n)
        {
            auto i(0);
            auto j(n - 1);

            if(n < 2) return;
            auto pivot(a[n / 2]);

            while(true)
            {
                while(a[i] < pivot) i++;
                while(pivot < a[j]) j--;

                if(i >= j) break;

                std::swap(a[i], a[j]);

                ++i;
                --j;
            }

            impl::sort_vector(a, i);
            impl::sort_vector(a + i, n - i);
        }

        template <typename T0>
        auto binsearch(const T0* a, int len, const T0& x)
        {
            if(len == 0) return -1;
            int mid = len / 2;

            if(a[mid] == x) return mid;

            if(a[mid] < x)
            {
                auto result(binsearch(a + mid + 1, len - (mid + 1), x));

                if(result == -1) return -1;

                return result + mid + 1;
            }

            if(a[mid] > x) return binsearch(a, mid, x);
        }

        template <typename T0>
        void insert_at(const T0& x, std::vector<T0>& vec)
        {
            auto old_size(vec.size());
            vec.resize(vec.size() + 1);

            int i(0);

            for(; i < old_size; ++i)
            {
                if(vec[i] < x) continue;

                for(int j(old_size); j > i; --j)
                {
                    vec[j] = vec[j - 1];
                }

                break;
            }

            vec[i] = x;
        }

        template <typename T0>
        void remove_at(int idx, std::vector<T0>& vec)
        {
            for(; idx < vec.size() - 1; ++idx)
            {
                vec[idx] = vec[idx + 1];
            }

            vec.pop_back();
        }
    }

    template <typename T0>
    auto& sort_vector(std::vector<T0>& vec)
    {
        impl::sort_vector(vec.data(), vec.size());
        return vec;
    }

    template <typename T0>
    auto find_in_sorted_vector(const T0& x, const std::vector<T0>& vec)
    {
        return impl::binsearch(vec.data(), vec.size(), x);
    }

    template <typename T0>
    void insert_in_sorted_vector(const T0& x, std::vector<T0>& vec)
    {
        impl::insert_at(x, vec);
    }

    template <typename T0>
    void remove_from_sorted_vector(int idx, std::vector<T0>& vec)
    {
        impl::remove_at(idx, vec);
    }
}

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
    private:
        std::array<T0, TRowCount * TColumnCount> _data;

        auto calc_index(std::size_t row, std::size_t column) const noexcept
        {
            assert(row < TRowCount);
            assert(column < TColumnCount);

            // std::cout << "(" << row << ", " << column << ")\n";

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
                f((*this)(i, j));
            }
        }

        template <typename TF>
        void for_column_idxs(std::size_t j, TF&& f)
        {
            for(auto i(0); i < TRowCount; ++i)
            {
                f((*this)(i, j));
            }
        }

        template <typename TF>
        void for_idxs(TF&& f)
        {
            for(auto i(0); i < TRowCount; ++i)
            {
                for(auto j(0); j < TColumnCount; ++j)
                {
                    f(i, j);
                }
            }
        }


        matrix(dont_init) {}
        matrix() { clear(); }
        matrix(init_identity) { clear_to_identity(); }

        matrix(const matrix& rhs) = default;
        matrix& operator=(const matrix& rhs) = default;

        matrix(matrix&& rhs) = default;
        matrix& operator=(matrix&& rhs) = default;

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

        auto& operator()(std::size_t row, std::size_t column)
        {
            return _data[calc_index(row, column)];
        }

        const auto& operator()(std::size_t row, std::size_t column) const
        {
            return _data[calc_index(row, column)];
        }

        template <typename T1>
        auto operator+(const matrix<T1, TRowCount, TColumnCount>& rhs) const
        {
            matrix<T1, TRowCount, TColumnCount> result{dont_init{}};

            for(std::size_t i(0); i < TRowCount; ++i)
                for(std::size_t j(0); j < TColumnCount; ++j)
                {
                    result(i, j) = (*this)(i, j) + rhs(i, j);
                }

            return result;
        }

        template <typename T1>
        auto operator-(const matrix<T1, TRowCount, TColumnCount>& rhs) const
        {
            matrix<T1, TRowCount, TColumnCount> result{dont_init{}};

            for(std::size_t i(0); i < TRowCount; ++i)
                for(std::size_t j(0); j < TColumnCount; ++j)
                {
                    result(i, j) = (*this)(i, j) - rhs(i, j);
                }

            return result;
        }

        template <typename T1, std::size_t TRhsColumnCount>
        auto operator*(
            const matrix<T1, TColumnCount, TRhsColumnCount>& rhs) const
        {
            matrix<T1, TRowCount, TRhsColumnCount> result{dont_init{}};

            std::cout << "result size: " << TRhsColumnCount << " * "
                      << TRhsColumnCount << "\n";

            for(std::size_t ri(0); ri < TRowCount; ++ri)
                for(std::size_t rj(0); rj < TRhsColumnCount; ++rj)
                {
                    using value_type = std::common_type_t<T0, T1>;
                    value_type x(0);

                    for(auto k(0); k < TColumnCount; ++k)
                    {
                        x += ((*this)(ri, k) * rhs(k, rj));
                    }

                    result(ri, rj) = x;
                }

            return result;
        }
    };

    template <typename T0, std::size_t TRowCount, std::size_t TColumnCount,
        typename... Ts>
    auto make_matrix(Ts&&... xs)
    {
        matrix<T0, TRowCount, TColumnCount> result;
        result.set_from_vector(xs...);
        return result;
    }

    template <typename T0, std::size_t TDim, typename... Ts>
    auto make_square_matrix(Ts&&... xs)
    {
        return make_matrix<T0, TDim, TDim>(xs...);
    }

    template <typename T0, std::size_t TDim, typename... Ts>
    auto make_identity_matrix()
    {
        return matrix<T0, TDim, TDim>{init_identity{}};
    }

    template <typename T0, typename... Ts>
    auto make_vector(Ts&&... xs)
    {
        return make_matrix<T0, 1, sizeof...(Ts)>(xs...);
    }

    template <typename T0, std::size_t TRowCount, std::size_t TColumnCount>
    auto make_transposed(const matrix<T0, TRowCount, TColumnCount>& m)
    {
        matrix<T0, TColumnCount, TRowCount> result;

        for(std::size_t i(0); i < TRowCount; ++i)
            for(std::size_t j(0); j < TColumnCount; ++j)
            {
                result(j, i) = m(i, j);
            }

        return result;
    }

    template <typename TV0, typename TV1>
    auto vector_scalar_product(const TV0& v0, const TV1& v1)
    {
        return v0 * make_transposed(v1);
    }

    template <typename TV0, typename TV1>
    auto vector_tensor_product(const TV0& v0, const TV1& v1)
    {
        return make_transposed(v0) * v1;
    }

    template <typename T0, std::size_t TDim>
    auto make_hilbert_matrix()
    {
        matrix<T0, TDim, TDim> result;

        result.for_idxs([&result](auto i, auto j)
            {
                result(i, j) = 1 / (i + j - 1);
            });

        return result;
    }

    template <typename T0, std::size_t TDim, typename TV0>
    auto make_vandermonde_matrix(const TV0& v)
    {
        matrix<T0, TDim, TDim> result;

        result.for_idxs([&v, &result](auto i, auto j)
            {
                result(i, j) = std::pow(v(0, i), j - 1);
            });

        return result;
    }

    template <typename T0, std::size_t TDim>
    auto calc_u_ij(
        const matrix<T0, TDim, TDim>& a, std::size_t i, std::size_t j)
    {
        auto sum = 0;

        if(i > 0)
            for(auto k(0); k < i - 1; ++k)
            {
                sum += a(i, k) * a(k, j);
            }


        return a(i, j) - sum;
    }

    template <typename T0, std::size_t TDim>
    auto calc_l_ij(
        const matrix<T0, TDim, TDim>& a, std::size_t i, std::size_t j)
    {
        auto sum = 0;

        if(j > 0)
            for(auto k(0); k < j - 1; ++k)
            {
                sum += (a(i, k) * a(k, j));
            }

        return (a(i, j) - sum) / a(j, j);
    }

    template <typename T0, std::size_t TDim>
    auto calc_lu_row(
        matrix<T0, TDim, TDim>& a, std::size_t fixed_i, std::size_t offset)
    {
        for(std::size_t j(offset); j < TDim; ++j)
        {
            // std::cout << "a(" << fixed_i << ", " << j
            //          << ") = " << calc_u_ij(a, fixed_i, j) << "\n";

            a(fixed_i, j) = calc_u_ij(a, fixed_i, j);
        }
    }

    template <typename T0, std::size_t TDim>
    auto calc_lu_col(
        matrix<T0, TDim, TDim>& a, std::size_t fixed_j, std::size_t offset)
    {
        for(std::size_t i(offset); i < TDim; ++i)
        {
            // std::cout << "a(" << i << ", " << fixed_j
            //           << ") = " << calc_l_ij(a, i, fixed_j) << "\n";

            a(i, fixed_j) = calc_l_ij(a, i, fixed_j);
        }
    }


    template <typename T0, std::size_t TDim>
    auto make_ul_decomposition(matrix<T0, TDim, TDim> m)
    {
        // matrix<T0, TDim, TDim> u, l;
        // u = m;
        //        matrix<T0, TDim, TDim> r = m;

        auto row_start(0);
        auto row_offset(0);
        auto col_start(0);
        auto col_offset(1);

        while(true)
        {
            if(row_start < TDim && row_offset < TDim)
            {
                /*
                                std::cout << "\n";

                                std::cout << "calc_lu_row:\n";
                                std::cout << "rs=" << row_start << ", ro=" <<
                   row_offset
                                          << "\n";
                */
                calc_lu_row(m, row_start, row_offset);
            }
            else
                break;

            ++row_start;
            ++row_offset;

            if(col_start < TDim && col_offset < TDim)
            {
                /*
                                std::cout << "\n";
                                std::cout << "calc_lu_col:\n";
                                std::cout << "cs=" << col_start << ", co=" <<
                   col_offset
                                          << "\n";
                */
                calc_lu_col(m, col_start, col_offset);
            }
            else
                break;

            ++col_start;
            ++col_offset;
        }

        return m;
    }
}
