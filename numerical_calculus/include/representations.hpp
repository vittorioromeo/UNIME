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

namespace nc
{
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
}
