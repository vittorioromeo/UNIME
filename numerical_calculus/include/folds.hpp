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

namespace nc
{
    // `fold` è un concetto della programmazione funzionale che esegue una
    // funzione su ogni valore di un'insieme e "collassa" il risultato di tale
    // funzione accumulandolo usando un'operazione definita dall'utente ed un
    // valore iniziale.
    template <typename T0, typename TF>
    auto fold(TF&& f, T0 initial_value, const std::vector<T0>& vec) // .
    // TODO: noexcept(noexcept(f(std::declval<T0>(), std::declval<T0>())))
    {
        T0 result(initial_value);
        for(const auto& x : vec) f(result, x);
        return result;
    }

    // La somma di un insieme di `n` numeri è definibile in termini di un `fold`
    // che utilzza l'operatore addizione.
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

    // La somma di un insieme di `n` numeri è definibile in termini di un `fold`
    // che utilzza l'operatore moltiplicazione.
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

    // La media aritmetica di `n` numeri è definita come la loro somma diviso
    // `n`.
    template <typename T0>
    auto average_n_numbers(const std::vector<T0>& vec)
    {
        return sum_n_numbers(vec) / vec.size();
    }

    // Il minimo di `n` numeri è definibile in termini di un fold che utilizza
    // una funzione `min` binaria come suo operatore.
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

    // Il minimo di `n` numeri è definibile in termini di un fold che utilizza
    // una funzione `max` binaria come suo operatore.
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
}
