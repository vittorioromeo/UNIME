#pragma once

#include <cstddef>
#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>

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
}
