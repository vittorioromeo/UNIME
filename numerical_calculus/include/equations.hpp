#pragma once

#include <cstddef>
#include <cassert>
#include <cmath>
#include <tuple>

namespace nc
{
    // Calcola il delta di un'equazione di secondo grado `ax^2 + bx + c`.
    template <typename T0, typename T1, typename T2>
    auto delta(T0 a, T1 b, T2 c) noexcept
    {
        auto x(std::pow(b, 2) - (4 * a * c));
        assert(x >= 0);

        return std::sqrt(x);
    }

    // Risolve un'equazione di secondo grado restituendo una tupla contentente
    // le due radici. Assume che l'equazione sia risolvibile.
    template <typename T0, typename T1, typename T2>
    auto solve_grade2_equation(T0 a, T1 b, T2 c) noexcept
    {
        assert(a != 0);

        auto my_delta(delta(a, b, c));

        // `solution` è una funzione lambda che calcola una radice utilizzando
        // il segno passato come argomento.
        auto solution([&my_delta, &a, &b](auto sign)
            {
                return (-b - (my_delta * sign)) / (2 * a);
            });

        auto x0(solution(1));
        auto x1(solution(-1));

        return std::make_tuple(x0, x1);
    }
}
