#pragma once

#include <cstddef>
#include <cassert>

namespace nc
{
    // Calcola il valore assoluto di `x`.
    template <typename T0>
    constexpr auto abs(T0 x) noexcept
    {
        return x < 0 ? -x : x;
    }

    // Calcola `a mod b`.
    template <typename T0, typename T1>
    constexpr auto mod(T0 a, T1 b) noexcept
    {
        assert(b != 0);

        a = abs(a);
        b = abs(b);

        auto division_result(a / b);
        auto multiplication_result(division_result * b);
        return a - multiplication_result;
    }

    // Restiusce `true` se `a` è un multiplo di `b`.
    template <typename T0, typename T1>
    constexpr auto is_multiple_of(T0 a, T1 b) noexcept
    {
        return mod(a, b) == 0;
    }

    // Restituisce `true` se `x` è pari.
    template <typename T0>
    constexpr auto is_even(T0 x) noexcept
    {
        return mod(x, 2) == 0;
    }

    // Restituisce `true` se `x` è dispari.
    template <typename T0>
    constexpr auto is_odd(T0 x) noexcept
    {
        return mod(x, 2) != 0;
    }
}
