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
#include "matrix.hpp"

namespace nc
{
    template <typename T0, std::size_t TComponentCount>
    using row_vector = matrix<T0, 1, TComponentCount>;

    template <typename T>
    decltype(auto) access_row_vector(T&& v, std::size_t j) noexcept
    {
        return v(0, j);
    }

    template <typename T0, std::size_t TComponentCount>
    using column_vector = matrix<T0, TComponentCount, 1>;

    template <typename T>
    decltype(auto) access_column_vector(T&& v, std::size_t i) noexcept
    {
        return v(i, 0);
    }

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
                result(i, j) = 1.f / ((i + 1.f) + (j + 1.f) - 1.f);
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

    namespace impl
    {
        template <typename TF, typename... Ts>
        void for_args(TF&& f, Ts&&... xs)
        {
            using intarray = int[];
            (void)intarray{(f(xs), 0)...};
        }
    }

    template <typename... Ts>
    auto make_diagonal_dominant_square_matrix(Ts&&... xs)
    {
        constexpr auto dim(sizeof...(xs));
        static_assert(dim > 0, "");

        using type = std::common_type_t<Ts...>;
        matrix<type, dim, dim> result{};

        std::size_t i(0);

        impl::for_args(
            [&result, &i](auto x)
            {
                result(i, i) = x;
                ++i;
            },
            xs...);

        return result;
    }

    template <typename T0, std::size_t TDim>
    auto make_crout_decomposition(const matrix<T0, TDim, TDim>& a)
    {
        auto n = TDim;

        matrix<T0, TDim, TDim> m;
        matrix<T0, TDim, TDim> u{init_identity{}};

        for(std::size_t i(0); i < n; ++i)
        {
            for(std::size_t j(i); j < n; ++j)
            {
                T0 sum(0);
                // for(std::size_t k(0); k < n; ++k)
                for(std::size_t k(0); k < i; ++k)
                {
                    sum += m(j, k) * u(k, i);
                }

                m(j, i) = a(j, i) - sum;
            }

            // for(std::size_t j(i + 1); j < n; ++j)
            for(std::size_t j(i); j < n; ++j)
            {
                T0 sum(0);
                // for(std::size_t k(0); k < n; ++k)
                for(std::size_t k(0); k < i; ++k)
                {
                    sum += m(i, k) * u(k, j);
                }

                assert(m(i, i) != 0);

                u(i, j) = (a(i, j) - sum) / m(i, i);
            }
        }

        return std::make_tuple(std::move(m), std::move(u));
    }

    namespace impl
    {

        template <typename T0, std::size_t TDim, typename TF>
        auto make_wilkinson_matrix_impl(TF&& f)
        {
            static_assert(is_odd(TDim), "");

            matrix<T0, TDim, TDim> m;

            // set diagonal
            for(auto k(0); k < TDim; ++k)
            {
                m(k, k) = f(k);
            }

            for(auto k(0); k < TDim - 1; ++k)
            {
                m(k + 1, k) = 1;
                m(k, k + 1) = 1;
            }

            return m;
        }
    }

    template <typename T0, std::size_t TDim>
    auto make_wilkinson_m_matrix()
    {
        return impl::make_wilkinson_matrix_impl<T0, TDim>([](auto k)
            {
                return std::floor((int)k - ((int)TDim / 2));
            });
    }

    template <typename T0, std::size_t TDim>
    auto make_wilkinson_p_matrix()
    {
        return impl::make_wilkinson_matrix_impl<T0, TDim>([](auto k)
            {
                return std::abs(std::floor((int)k - ((int)TDim / 2)));
            });
    }

    auto lagrange_interpolator(
        const std::vector<float>& x, const std::vector<float>& fx)
    {
        return [x, fx](float value)
        {
            if(fx.size() != x.size() || (x.size() == 0))
            {
                return 0.0;
            }

            float result = 0;

            float up, down;
            for(int i = 0; i < x.size(); i++)
            {
                up = down = 1;
                for(int j = 0; j < x.size(); j++)
                {
                    if(j != i)
                    {
                        up *= (value - x[j]);
                        down *= (x[i] - x[j]);
                    }
                }
                result += ((up / down) * fx[i]);
            }

            return result;
        };
    }
}
