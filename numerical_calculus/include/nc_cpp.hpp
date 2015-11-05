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

    // TODO:
    // * fix extra subtraction in modulus
    // * complex results in 2nd grade equation

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
            T0 i(1), last_n(1), n(1);

            while(f(last_n, n))
            {
                ++i;
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
}
