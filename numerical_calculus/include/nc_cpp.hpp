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

#undef ARMA_USE_ATLAS
#include <armadillo>

#include "utilities.hpp"
#include "multiples.hpp"
#include "equations.hpp"
#include "folds.hpp"
#include "representations.hpp"
#include "sorted_vector.hpp"
#include "matrix.hpp"

namespace nc
{
    // I nostri vettori sono implementati come matrici.
    // Definiamo qui due alias per i tipi di matrici che rappresentano i
    // vettori, uno per colonna, uno per riga.

    // Alias per vettore riga (1 riga, N colonne).
    // `T0` è il tipo di dato.
    template <typename T0, std::size_t TComponentCount>
    using row_vector = matrix<T0, 1, TComponentCount>;

    // Alias per vettore colonna (N righe, 1 colonna).
    template <typename T0, std::size_t TComponentCount>
    using column_vector = matrix<T0, TComponentCount, 1>;

    // Per evitare errori durante l'accesso ai dati dei vettori, definiamo due
    // funzioni che permettono di ottenere la n-esima componente.

    // Accedi alla j-esima componente di un vettore riga.
    template <typename T>
    decltype(auto) access_row_vector(T&& v, std::size_t j) noexcept
    {
        return v(0, j);
    }

    // Accedi alla i-esima componente di un vettore colonna.
    template <typename T>
    decltype(auto) access_column_vector(T&& v, std::size_t i) noexcept
    {
        return v(i, 0);
    }

    // Restituisce la dimensione di un vettore.
    template <typename T>
    constexpr auto vector_dimension(const T& v)
    {
        constexpr auto rc = v.row_count();
        constexpr auto cc = v.column_count();

        static_assert(rc == 1 || cc == 1, "");
        return rc < cc ? cc : rc;
    }

    // Crea una matrice NxM da NxM argomenti variadici.
    template <typename T0, std::size_t TRowCount, std::size_t TColumnCount,
        typename... Ts>
    auto make_matrix(Ts&&... xs)
    {
        matrix<T0, TRowCount, TColumnCount> result;

        // TODO:
        result.set_from_variadic(xs...);

        return result;
    }

    // Crea un matrice quadrata da N argomenti variadici.
    template <typename T0, std::size_t TDim, typename... Ts>
    auto make_square_matrix(Ts&&... xs)
    {
        return make_matrix<T0, TDim, TDim>(xs...);
    }

    // Crea un matrice identità NxN.
    template <typename T0, std::size_t TDim, typename... Ts>
    auto make_identity_matrix()
    {
        return matrix<T0, TDim, TDim>{init_identity{}};
    }

    // Crea un vettore riga (matrice 1xN).
    template <typename T0, typename... Ts>
    auto make_row_vector(Ts&&... xs)
    {
        return make_matrix<T0, 1, sizeof...(Ts)>(xs...);
    }

    // Crea un vettore colonna (matrice Nx1).
    template <typename T0, typename... Ts>
    auto make_column_vector(Ts&&... xs)
    {
        return make_matrix<T0, sizeof...(Ts), 1>(xs...);
    }

    // Data una matrice `m`, restituisce la sua trasposta.
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

    // Esegue il prodotto scalare tra due vettori.
    template <typename TV0, typename TV1>
    auto vector_scalar_product(const TV0& v0, const TV1& v1)
    {
        return v0 * make_transposed(v1);
    }

    // Esegue il prodotto tensoriale tra due vettori.
    template <typename TV0, typename TV1>
    auto vector_tensor_product(const TV0& v0, const TV1& v1)
    {
        return make_transposed(v0) * v1;
    }

    // Genera la matrice di Hilbert.
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

    // Dettagli di implementazione per la matrice di Vandermonde.
    namespace impl
    {
        template <typename T0, std::size_t TRows, std::size_t TCols,
            typename TVAccess, typename TV0>
        auto make_vandermonde_matrix(TVAccess&& v_access, const TV0& v)
        {
            matrix<T0, TRows, TCols> result;

            // Assume TRows x TRows
            for(auto i(0); i < TRows; ++i)
                for(auto j(0); j < TRows; ++j)
                {
                    result(i, j) = std::pow(v_access(v, i), j);
                }

            return result;
        }
    }

    // Genera la matrice di Vandermonde.
    template <typename T0, std::size_t TDim, typename TV0>
    auto make_vandermonde_matrix(const TV0& v)
    {
        return impl::make_vandermonde_matrix<T0, TDim, TDim>(
            [](auto& v, auto i)
            {
                return access_row_vector(v, i);
            },
            v);
    }

    // Genera una matrice diagonale dominante.
    template <typename... Ts>
    auto make_diagonal_dominant_square_matrix(Ts&&... xs)
    {
        constexpr auto dim(sizeof...(xs));
        static_assert(dim > 0, "");

        using type = std::common_type_t<Ts...>;
        matrix<type, dim, dim> result{};

        std::size_t i(0);

        for_args(
            [&result, &i](auto x)
            {
                result(i, i) = x;
                ++i;
            },
            xs...);

        return result;
    }

    // Esegue la decomposizione LU di Crout.
    template <typename T0, std::size_t TDim>
    auto make_crout_decomposition(const matrix<T0, TDim, TDim>& a)
    {
        auto n = TDim;

        matrix<T0, TDim, TDim> l;
        matrix<T0, TDim, TDim> u{init_identity{}};

        // Itera sulle righe.
        for(std::size_t i(0); i < n; ++i)
        {
            // Itera sulle colonne, partendo da `i`.
            for(std::size_t j(i); j < n; ++j)
            {
                // Sommatoria.
                T0 sum(0);
                for(std::size_t k(0); k < i; ++k)
                {
                    sum += l(j, k) * u(k, i);
                }

                // Setta valore della matrice L.
                l(j, i) = a(j, i) - sum;
            }

            // Itera sulle colonne, partendo da `i`.
            for(std::size_t j(i); j < n; ++j)
            {
                // Sommatoria.
                T0 sum(0);
                for(std::size_t k(0); k < i; ++k)
                {
                    sum += l(i, k) * u(k, j);
                }

                // Setta valore della matrice U.
                assert(l(i, i) != 0);
                u(i, j) = (a(i, j) - sum) / l(i, i);
            }
        }

        // Restituisci una tupla (l, u).
        return std::make_tuple(std::move(l), std::move(u));
    }

    // Dettagli di implementazione matrice di Wilkinson.
    namespace impl
    {
        template <typename T0, std::size_t TDim, typename TF>
        auto make_wilkinson_matrix_impl(TF&& f)
        {
            // L'ordine deve essere dispari.
            static_assert(is_odd(TDim), "");

            matrix<T0, TDim, TDim> m;

            // Setta la diagonale.
            for(auto k(0); k < TDim; ++k)
            {
                m(k, k) = f(k);
            }

            // Setta valori adiacenti alla diagonale principale.
            for(auto k(0); k < TDim - 1; ++k)
            {
                m(k + 1, k) = 1;
                m(k, k + 1) = 1;
            }

            return m;
        }

        // Calcolo di default (con segno) della matrice di Wilkinson.
        template <std::size_t TDim>
        auto make_wilkinson_calc()
        {
            return [](auto k)
            {
                return std::floor((int)k - ((int)TDim / 2));
            };
        }
    }

    // Genera una matrice di Wilkinson con il segno.
    template <typename T0, std::size_t TDim>
    auto make_wilkinson_m_matrix()
    {
        auto calc = impl::make_wilkinson_calc<TDim>();
        return impl::make_wilkinson_matrix_impl<T0, TDim>(calc);
    }

    // Genera una matrice di Wilkinson con valore assoluto.
    template <typename T0, std::size_t TDim>
    auto make_wilkinson_p_matrix()
    {
        // Compone la funzione `abs` con `calc`.
        auto calc = impl::make_wilkinson_calc<TDim>();
        auto wrapped_calc = [&calc](auto k)
        {
            return std::abs(calc(k));
        };

        return impl::make_wilkinson_matrix_impl<T0, TDim>(wrapped_calc);
    }

    // Genera un interpolatore di Lagrange.
    template <typename TVector>
    auto lagrange_interpolator(const TVector& x, const TVector& fx)
    {
        // Verifica validità dei campioni.
        static_assert(vector_dimension(x) == vector_dimension(fx), "");
        static_assert(vector_dimension(x) > 0, "");

        // Restituisce la funzione interpolante.
        return [x, fx](float value)
        {
            // Numero dei campioni.
            constexpr auto x_size = vector_dimension(x);

            // Accumulatore del risultato.
            float result = 0;

            // Esecuzione formula.
            float num, den;
            for(int i = 0; i < x_size; i++)
            {
                num = den = 1;

                // Salta caso `i == j`.
                loop_skipping(0, x_size, i, [&num, &den, &x, i, value](auto j)
                    {
                        num *= (value - access_column_vector(x, j));
                        den *= (access_column_vector(x, i) -
                                access_column_vector(x, j));
                    });

                result += (num / den) * access_column_vector(fx, i);
            }

            return result;
        };
    }

    // Genera un interpolatore monomiale.
    template <std::size_t TN, typename TXV, typename TFXV>
    auto monomial_interpolator(const TXV& x, const TFXV& fx)
    {
        // Verifica validità dei campioni.
        static_assert(vector_dimension(x) == vector_dimension(fx), "");
        static_assert(vector_dimension(x) > 0, "");

        // Crea matrice di Vandermonde con grado `TN`.
        // La matrice rappresenta un sistema lineare.
        // I coefficenti della matrice sono ottenuti dal vettore `x`.
        auto vmm = impl::make_vandermonde_matrix<float, TN, TN + 1>(
            [](auto& v, auto i)
            {
                return access_column_vector(v, i);
            },
            x);

        // Setta i termini noti del sistema lineare di Vandermonde.
        for(auto k(0); k < TN; ++k)
        {
            vmm(k, TN) = access_column_vector(fx, k);
        }

        // Risolvi il sistema con il metodo Gauss.
        // Troviamo così i coefficenti del polinomio interpolatore.
        auto solved_vmm = vmm.solve_gauss();

        // TODO:
        /*
        for(auto k(0); k < TN; ++k)
        {
            std::cout << access_column_vector(solved_vmm, k) << "\n";
        }
        */

        // Restiusce la funzione interpolatrice.
        return [=](auto value)
        {
            // Accumulatore del risultato.
            float acc = 0;

            for(auto k(0); k < TN; ++k)
            {
                // `k`-esimo coefficiente.
                auto coeff = solved_vmm(0, k);

                // Calcola il monomio di grado `k`.
                acc += coeff * std::pow(value, k);
            }

            return acc;
        };
    }

    template <typename TVector>
    auto newton_interpolator(TVector x, TVector fx)
    {
        // Verifica validità dei campioni.
        static_assert(vector_dimension(x) == vector_dimension(fx), "");
        static_assert(vector_dimension(x) > 0, "");

        // Numero di campioni.
        constexpr auto n = vector_dimension(x);

        // Tabella delle differenze divise.
        matrix<double, n, n + 1> table;

        // Generazione della tabella: prime due colonne.
        for(int i = 0; i < n; ++i)
        {
            // Colonna 0: valori di `x`.
            table(i, 0) = access_column_vector(x, i);

            // Colonna 1: valori di `fx`.
            table(i, 1) = access_column_vector(fx, i);
        }

        // Generazione della tabella: resto delle colonne.
        // Esegue l'algoritmo delle differenze divise.
        for(int c = 2; c < n + 1; ++c)
        {
            // Riga corrente.
            int curr = 0;

            for(int r = c - 1; r < n; ++r)
            {
                // Indici precedenti riga/colonna.
                auto prev_r = r - 1;
                auto prev_c = c - 1;

                // Valori precedenti numeratore.
                auto prev0 = table(r, prev_c);
                auto prev1 = table(prev_r, prev_c);

                // Differenza dei valori precedenti.
                // (Numeratore.)
                auto num = prev0 - prev1;

                // Valori precedenti denominatore.
                auto den_v0 = table(r, 0);
                auto den_v1 = table(curr, 0);

                // Differenza dei valori precedenti.
                // (Denominatore.)
                auto den = den_v0 - den_v1;

                // Divisione delle differenze.
                auto frac = num / den;

                // TODO:
                // std::cout << r << ", " << c << " -> " << frac << "\n";

                table(r, c) = frac;
                ++curr;
            }
        }

        // TODO:
        /*
        for(int ix = 0; ix < n; ++ix)
        {
            for(int iy = 0; iy < n + 1; ++iy)
            {
                std::cout << table(ix, iy) << "\t";
            }

            std::cout << "\n";
        }
        */

        // Restituisce la funzione interpolatrice.
        return [n, table, x](auto value)
        {
            // Restituisce l'`u`-esimo coefficiente.
            auto uesimo = [=](auto u)
            {
                // TODO:
                /*
                    std::cout << "uesimo(" << u << ") = " << table(u, u + 1)
                              << "\n";
                */

                return table(u, u + 1);
            };

            // Accumulatore del risultato.
            double acc = 0;

            // Itera sul grado.
            for(int i = 0; i < n; ++i)
            {
                // Accumulatore dei fattori interni.
                double pacc = 1;
                for(int j = 0; j < i; ++j)
                {
                    pacc *= value - access_column_vector(x, j);
                }

                acc += uesimo(i) * pacc;

                // Esempio:
                // `(x-x0)(x-x1) * f[x0,x1,x2]`
                // Il `pacc` equivale a: `(x-x0)(x-x1)`.
                // L'`uesimo` equivale a: `f[x0,x1,x2]`.
            }

            return acc;
        };
    }
}

// TODO: file per l'interpolazione
