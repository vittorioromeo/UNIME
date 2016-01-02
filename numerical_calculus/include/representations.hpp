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
    namespace impl
    {
        // Funzione di implementazione per il calcolo del fattoriale massimo
        // rappresentabile nel tipo numerico `T0`.
        template <typename T0, typename TF>
        auto max_representable_factorial(TF&& f) noexcept
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

    // Calcolo del massimo fattoriale rappresentabile in un tipo intero `T0`.
    // Il ciclo di implementazione viene eseguito mentre `last_n <= n`.
    template <typename T0>
    auto max_representable_factorial()
    {
        return impl::max_representable_factorial<T0>(
            [](const auto& last_n, const auto& n)
            {
                return last_n <= n;
            });
    }

    // Calcolo del massimo fattoriale rappresentabile in un tipo reale `T0`.
    // Il ciclo di implementazione viene eseguito mentre il numero è un reale
    // valido.
    template <typename T0>
    auto max_representable_factorial_real() noexcept
    {
        return impl::max_representable_factorial<T0>(
            [](const auto&, const auto& n)
            {
                return !std::isnan(n) && !std::isinf(n);
            });
    }

    // Trova la precisione di macchina del tipo reale `T0`.
    // Utilizza un numero temporaneo `curr`, che viene continuamente diviso per
    // `10`, fintanto che `1 != 1 + curr`.
    template <typename T0>
    auto find_epsilon() noexcept
    {
        T0 curr(0);

        for(int i(2); T0(1) + curr != T0(1); ++i)
        {
            curr = T0(1) / T0(10 * i);
        }

        return curr * 2;
    }
}
