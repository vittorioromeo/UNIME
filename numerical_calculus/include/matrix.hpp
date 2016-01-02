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

namespace nc
{
    // Tipo vuoto per segnalare al costruttore della classe `matrix` che essa
    // deve essere inizializzata come matrice identità.
    struct init_identity
    {
    };

    // Tipo vuoto per segnalare al costruttore della classe `matrix` che essa
    // non deve essere inizializzata.
    struct dont_init
    {
    };

    // Classe che rappresenta una matrice di tipo `T0` le cui dimensioni sono
    // conosciute a tempo di compilazione.
    template <typename T0, std::size_t TRowCount, std::size_t TColumnCount>
    class matrix
    {
    public:
        // Restituisce il numero delle righe.
        constexpr static auto row_count() noexcept { return TRowCount; }

        // Restituisce il numero delle colonne.
        constexpr static auto column_count() noexcept { return TColumnCount; }

    private:
        // Dati della matrice: conservati in un `array` unidimensionale
        // standard.
        std::array<T0, TRowCount * TColumnCount> _data;

        // Dati un indice di riga, ed un indice di colonna, calcola
        // l'equivalente indice unidimensionale per accedere all'array dei dati.
        auto calc_index(std::size_t row, std::size_t column) const noexcept
        {
            assert(row < row_count());
            assert(column < column_count());

            return column + column_count() * row;
        }

        // Dato un indice unidimensionale, restituisce una tupla di indici
        // bidimensionali equivalenti all'accesso corretto all'array dei dati.
        auto calc_1d_index(std::size_t i) const noexcept
        {
            assert(i >= 0 && column_count() != 0);

            auto y(i / column_count());
            return std::make_tuple(y, i - y * column_count());
        }

        // Implementazione dell'iterazione sugli indici di una riga della
        // matrice.
        // (Fissa la riga, ed itera su essa).
        template <typename TSelf, typename TF>
        static void impl_for_row_idxs(TSelf&& self, std::size_t i, TF&& f)
        {
            constexpr auto self_column_count(self.column_count());
            for(auto j(0); j < self_column_count; ++j)
            {
                f(i, j);
            }
        }

        // Implementazione dell'iterazione sugli indici di una colonna della
        // matrice.
        // (Fissa la colonna, ed itera su essa).
        template <typename TSelf, typename TF>
        static void impl_for_column_idxs(TSelf&& self, std::size_t j, TF&& f)
        {
            constexpr auto self_row_count(self.row_count());
            for(auto i(0); i < self_row_count; ++i)
            {
                f(i, j);
            }
        }

        // Implementazione dell'iterazione su tutti gli indici di una matrice.
        template <typename TSelf, typename TF>
        static void impl_for_idxs(TSelf&& self, TF&& f)
        {
            constexpr auto self_row_count(self.row_count());
            constexpr auto self_column_count(self.column_count());

            for(auto i(0); i < self_row_count; ++i)
                for(auto j(0); j < self_column_count; ++j)
                {
                    f(i, j);
                }
        }

        template <typename TSelf, typename TTpl>
        static auto& impl_at(TSelf&& self, const TTpl& t)
        {
            return self(std::get<0>(t), std::get<1>(t));
        }

    public:
        // Restituisce `true` se la matrice è quadrata.
        constexpr auto static is_square() noexcept
        {
            return row_count() == column_count();
        }

        // Esegue una funzione `f` su tutti gli indici della matrice.
        template <typename TF>
        void for_idxs(TF&& f)
        {
            impl_for_idxs(*this, FWD(f));
        }
        template <typename TF>
        void for_idxs(TF&& f) const
        {
            impl_for_idxs(*this, FWD(f));
        }

        // Esegue una funzione `f` su tutti gli indici di una riga.
        // (Fissa la riga, ed itera su essa).
        template <typename TF>
        void for_row_idxs(std::size_t i, TF&& f)
        {
            impl_for_row_idxs(*this, i, FWD(f));
        }
        template <typename TF>
        void for_row_idxs(std::size_t i, TF&& f) const
        {
            impl_for_row_idxs(*this, i, FWD(f));
        }

        // Esegue una funzione `f` su tutti gli indici di una colonna.
        // (Fissa la colonna, ed itera su essa).
        template <typename TF>
        void for_column_idxs(std::size_t j, TF&& f)
        {
            impl_for_column_idxs(*this, j, FWD(f));
        }
        template <typename TF>
        void for_column_idxs(std::size_t j, TF&& f) const
        {
            impl_for_column_idxs(*this, j, FWD(f));
        }

        // Pulisce la matrice, settando tutti i valori a `0`.
        void clear()
        {
            for_idxs([this](auto i, auto j)
                {
                    (*this)(i, j) = 0;
                });
        }

        // Pulisce la matrice, settandola come matrice identità. La matrice deve
        // essere quadrata.
        void clear_to_identity()
        {
            static_assert(is_square(), "");

            clear();

            for(auto k(0); k < row_count(); ++k)
            {
                (*this)(k, k) = 1;
            }
        }

        // Construttori.
        matrix(dont_init) {}
        matrix() { clear(); }
        matrix(init_identity) { clear_to_identity(); }

        // Operazioni di copia (default).
        matrix(const matrix& rhs) = default;
        matrix& operator=(const matrix& rhs) = default;

        // Operazioni di spostamento (default).
        matrix(matrix&& rhs) = default;
        matrix& operator=(matrix&& rhs) = default;

        // Restituisce il valore della matrice contenuto negli indici
        // rappresentati da una tupla.
        template <typename TTpl>
        auto& at(const TTpl& t)
        {
            return impl_at(*this, t);
        }
        template <typename TTpl>
        const auto& at(const TTpl& t) const
        {
            return impl_at(*this, t);
        }

        // Riempie i valori della matrice da un numero variadico di argomenti.
        template <typename... Ts>
        void set_from_variadic(Ts&&... xs)
        {
            // Controlla che il numero di argomenti sia valido.
            static_assert(sizeof...(xs) == row_count() * column_count(), "");

            // TODO:
            std::vector<T0> vec{xs...};
            for(auto i(0u); i < vec.size(); ++i)
            {
                (*this).at(calc_1d_index(i)) = vec[i];
            }
        }

        // Restituisce il valore presente negli indici passati.
        auto& operator()(std::size_t row, std::size_t column)
        {
            return _data[calc_index(row, column)];
        }
        const auto& operator()(std::size_t row, std::size_t column) const
        {
            return _data[calc_index(row, column)];
        }

    private:
        // Implementazione di operazioni semplici (somma, sottrazione) tra due
        // matrici della stessa grandezza.
        template <typename T1, typename TF>
        auto impl_simple_binary_op(
            const matrix<T1, TRowCount, TColumnCount>& rhs, TF&& f) const
        {
            // Tipo in comune tra i valori delle due matrici.
            using common = std::common_type_t<T0, T1>;

            // Alloca una nuova matrice non-inizializzata.
            matrix<common, TRowCount, TColumnCount> result{dont_init{}};

            // Per ogni indice delle matrici, esegue la funzione passata.
            // Converva il risultato nella nuova matrice.
            for_idxs([this, &rhs, &result, &f](auto i, auto j)
                {
                    result(i, j) = f((*this)(i, j), rhs(i, j));
                });

            return result;
        }

    public:
        // Overload dell'operatore `+` per la somma di due matrici.
        template <typename T1>
        auto operator+(const matrix<T1, TRowCount, TColumnCount>& rhs) const
        {
            return impl_simple_binary_op(rhs, [](const auto& lv, const auto& rv)
                {
                    return lv + rv;
                });
        }

        // Overload dell'operatore `-` per la sottrazione di due matrici.
        template <typename T1>
        auto operator-(const matrix<T1, TRowCount, TColumnCount>& rhs) const
        {
            return impl_simple_binary_op(rhs, [](const auto& lv, const auto& rv)
                {
                    return lv - rv;
                });
        }

        // Overload dell'operatore `==` per l'uguaglianza di due matrici.
        template <typename T1>
        auto operator==(const matrix<T1, TRowCount, TColumnCount>& rhs) const
        {
            for(auto i(0); i < row_count(); ++i)
                for(auto j(0); j < column_count(); ++j)
                    if((*this)(i, j) != rhs(i, j))
                    {
                        return false;
                    }

            return true;
        }

        // Overload dell'operatore `!=` per la disuguaglianza di due matrici.
        template <typename T1>
        auto operator!=(const matrix<T1, TRowCount, TColumnCount>& rhs) const
        {
            return !(*this == rhs);
        }

        // Overload dell'operatore `*` per la moltiplicazione di due matrici.
        template <typename T1, std::size_t TRhsColumnCount>
        auto operator*(
            const matrix<T1, TColumnCount, TRhsColumnCount>& rhs) const
        {
            // Tipo in comune tra i valori delle due matrici.
            using common = std::common_type_t<T0, T1>;

            // Alloca una nuova matrice non-inizializzata.
            matrix<common, TRowCount, TRhsColumnCount> result{dont_init{}};

            // Itera su ogni indice della nuova matrice.
            for(std::size_t ri(0); ri < TRowCount; ++ri)
                for(std::size_t rj(0); rj < TRhsColumnCount; ++rj)
                {
                    // Accumulatore per il valore a `(ri, rj)`.
                    common x(0);

                    // Step di accumulazione.
                    for(auto k(0); k < TColumnCount; ++k)
                    {
                        x += ((*this)(ri, k) * rhs(k, rj));
                    }

                    // Setta il valore nella nuova matrice.
                    result(ri, rj) = x;
                }

            return result;
        }

    private:
        // Restituisci la somma dei valore assoluti in una colonna.
        auto sum_abs_value_in_column(std::size_t j) const noexcept
        {
            T0 res(0);
            for(auto i(0); i < TRowCount; ++i)
            {
                res += abs((*this)(i, j));
            }

            return res;
        }

        // Restituisci la somma dei valore assoluti in una riga.
        auto sum_abs_value_in_row(std::size_t i) const noexcept
        {
            T0 res(0);
            for(auto j(0); j < TColumnCount; ++j)
            {
                res += abs((*this)(i, j));
            }

            return res;
        }

        // Converte la rappresentazione della matrice a quella della libreria
        // "Eigen", per il calcolo della "norma di Frobenius".
        auto as_mapped_eigen() const noexcept
        {
            using eigen_matrix_type =
                Eigen::Matrix<T0, TRowCount, TColumnCount>;

            return Eigen::Map<const eigen_matrix_type>(
                _data.data(), TRowCount, TColumnCount);
        }

    public:
        // Calcolo della norma 1.
        auto norm_1() const noexcept
        {
            T0 curr_max(0);
            for(auto j(0); j < TColumnCount; ++j)
            {
                curr_max = std::max(curr_max, sum_abs_value_in_column(j));
            }

            return curr_max;
        }

        // Calcolo della norma infinita.
        auto norm_inf() const noexcept
        {
            T0 curr_max(0);
            for(auto i(0); i < TRowCount; ++i)
            {
                curr_max = std::max(curr_max, sum_abs_value_in_row(i));
            }

            return curr_max;
        }

        // Calcolo della norma 2.
        // (Utilizza la libreria "armadillo".)
        auto norm_2() const noexcept
        {
            std::vector<double> v;
            for(const auto& x : _data) v.emplace_back(x);

            auto m = arma::mat::fixed<TRowCount, TColumnCount>(v.data());
            return arma::norm(m, 2);
        }

        // Calcolo della norma 2.
        // (Utilizza la libreria "Eigen".)
        auto norm_frobenius() const noexcept
        {
            // TODO: implement manually
            return as_mapped_eigen().norm();
        }

        // Restituisce l'ordine della matrice. Se la matrice non è quadrata,
        // restituisce la dimensione minore.
        constexpr auto static order() noexcept
        {
            return column_count() < row_count() ? column_count() : row_count();
        }

        // Calcolo del minore di una matrice (taglia una riga ed una colonna).
        auto calc_minor(
            std::size_t row_to_skip, std::size_t column_to_skip) const noexcept
        {
            // Alloca la matrice minore (ordine inferiore di 1).
            matrix<T0, order() - 1, order() - 1> result{dont_init{}};

            // Prossimi indici da riempire nella matrice nuova.
            std::size_t target_column(0);
            std::size_t target_row(0);

            // Iteriamo sulle nostre righe.
            for(auto i(0); i < order(); ++i)
            {
                // TODO: remove check?
                // Saltiamo la riga desiderata.
                if(i == row_to_skip) continue;

                // Iteriamo sulle nostre colonne.
                target_column = 0;
                for(auto j(0); j < order(); ++j)
                {
                    // TODO: remove check?
                    // Saltiamo la colonna desiderata.
                    if(j == column_to_skip) continue;

                    // Settiamo il valore nella matrice nuova.
                    result(target_row, target_column) = (*this)(i, j);
                    ++target_column;
                }

                ++target_row;
            }

            return result;
        }

        // Calcolo del determinante.
        auto determinant() const noexcept
        {
            using namespace vrm::core;

            // Usiamo un `if` a tempo di compilazione per fermare la ricorsione
            // nel caso base in cui il nostro ordine sia uguale a `1`.
            return static_if(bool_v<(order() == 1)>)
                .then([](const auto& x)
                    {
                        // Se l'ordine è uguale a `1`, restitusci il valore in
                        // alto a sinistra.
                        return x(0, 0);
                    })
                .else_([](const auto& x)
                    {
                        // Altrimenti...
                        T0 result(0);

                        // Itera sulle nostre righe.
                        for(int i = 0; i < order(); i++)
                        {
                            // Prendi il minore tagliando la prima riga e
                            // l'i-esima colonna.
                            auto my_minor(x.calc_minor(0, i));

                            // Calcola il segno del prossimo coefficiente.
                            auto sign(i % 2 == 1 ? -1.0 : 1.0);

                            // Aggiungi il coefficiente per il determinante del
                            // minore all'accumulatore. (Ricorsione.)
                            result += sign * x(0, i) * my_minor.determinant();
                        }

                        return result;
                    })(*this);
        }

        // Calcola la matrice inversa.
        auto inverse() const noexcept
        {
            // Alloca matrice vuota della stessa dimensione.
            matrix<T0, TRowCount, TColumnCount> result{dont_init{}};

            using namespace vrm::core;

            // Usiamo un `if` a tempo di compilazione per il caso speciale in
            // cui il nostro ordine è uguale a `1`.
            static_if(bool_v<(order() == 1)>)
                .then([&result](const auto& x)
                    {
                        // Se l'ordine è uguale a `1`, setta il valore in alto a
                        // sinistra della matrice risultato al reciproco di
                        // quello della matrice corrente.
                        result(0, 0) = 1.0 / x(0, 0);
                    })
                .else_([&result](const auto& x)
                    {
                        // Altrimenti, calcola il reciproco del determinante.
                        auto rec_det(1.0 / x.determinant());

                        // Itera sulle colonne.
                        for(int j = 0; j < x.order(); j++)
                        {
                            // Itera sulle righe.
                            for(int i = 0; i < x.order(); i++)
                            {
                                // Calcola il minore tagliando la riga j e la
                                // colonna i.
                                auto my_minor(x.calc_minor(j, i));

                                // Calcola il segno del prossimo coefficiente.
                                auto sign((i + j) % 2 == 1 ? -1.0 : 1.0);

                                // Setta il valore della matrice risultato.
                                result(i, j) =
                                    sign * rec_det * my_minor.determinant();
                            }
                        }
                    })(*this);

            return result;
        }

        // Calcola l'indice di perturbazione.
        auto perturbation_index() const noexcept
        {
            auto result(norm_2() * inverse().norm_2());

            assert(result >= 1);
            return result;
        }

        // Risolve un sistema lineare usando il metodo di eliminazione di Gauss.
        auto solve_gauss() const noexcept
        {
            // La matrice deve avere una colonna in più delle righe.
            // (La colonna dei termini noti.)
            static_assert(column_count() == row_count() + 1, "");
            constexpr auto n(row_count());

            // Creiamo una copia della matrice corrente.
            auto a(*this);

            // Itera sull'ordine.
            for(std::size_t i(0); i < n; i++)
            {
                // Cerca l'indice del valore assoluto massimo nella colonna i.
                auto max_element(abs(a(i, i)));
                auto max_row_index(i);

                // Itera sulle righe (iteratore `k`).
                for(auto k(i + 1); k < n; k++)
                {
                    auto curr_element(abs(a(k, i)));
                    if(curr_element > max_element)
                    {
                        // Aggiorna il pivot.
                        max_element = curr_element;
                        max_row_index = k;
                    }
                }

                // Scambia riga dell'elemento massimo con la riga corrente.
                // (Colonna per colonna.)
                for(auto k(i); k < n + 1; k++)
                {
                    std::swap(a(max_row_index, k), a(i, k));
                }

                // Rende `0` tutti gli elementi sotto il pivot corrente.
                for(auto k(i + 1); k < n; k++)
                {
                    // TODO: review with pseudocode

                    auto c(-a(k, i) / a(i, i));
                    for(auto j(i); j < n + 1; j++)
                    {
                        if(i == j)
                        {
                            a(k, j) = 0;
                        }
                        else
                        {
                            a(k, j) += c * a(i, j);
                        }
                    }
                }
            }

            // Alloca un vettore per i risultati.
            matrix<T0, n, 1> x;

            // Risolvi l'equazione `Ax=b`.
            // La matrice `a` adesso è triangolare superiore.
            for(int i(n - 1); i >= 0; i--)
            {
                x(i, 0) = a(i, n) / a(i, i);
                for(int k(i - 1); k >= 0; k--)
                {
                    a(k, n) -= a(k, i) * x(i, 0);
                }
            }

            return x;
        }

        // Risolvi un sistema lineare tramite il metodo di Jacobi.
        auto solve_jacobi() const noexcept
        {
            // La matrice deve avere una colonna in più delle righe.
            // (La colonna dei termini noti.)
            static_assert(column_count() == row_count() + 1, "");
            constexpr auto n(TRowCount);

            // Tipo per gestire la precisione.
            using accuracy_type = double;

            // Precisione.
            accuracy_type accuracy(0.0001);

            // Divergenza massima corrente.
            accuracy_type max_divergence(0);

            // TODO:
            std::vector<double> newSolution(n, 0);
            std::vector<double> solution = newSolution;

            do
            {
                // Inizialmente setta la divergenza massima come il minor
                // possibile valore rapprentabile.
                max_divergence = std::numeric_limits<accuracy_type>::lowest();

                // Itera sulle righe.
                for(int i = 0; i < row_count(); i++)
                {
                    // Prende l'i-esimo valore della riga.
                    auto x((*this)(i, n));

                    // Itera sulle colonne.
                    for(int j = 0; j < n; j++)
                    {
                        // Se `i == j`, salta l'iterazione.
                        if(i == j) continue;

                        // TODO:
                        x -= (*this)(i, j) * solution[j];
                    }

                    x /= (*this)(i, i);
                    newSolution[i] = x;

                    // Calcola la divergenza tra la soluzione corrente e quella
                    // precedente.
                    auto divergence(std::fabs(solution[i] - newSolution[i]));

                    // Aggiorna la divergenza massima se necessario.
                    max_divergence = std::max(max_divergence, divergence);
                }


                // TODO:
                for(auto j : solution) std::cout << j << " ";
                std::cout << "\n divergence:  " << max_divergence << "\n\n";

                // Sostituisce la soluzione precendente con quella corrente.
                solution = newSolution;
            } while(max_divergence > accuracy);

            // TODO: verificare divergenza -> no soluzioni

            return solution;
        }

        template <typename TSolutions>
        auto solve_gauss_seidel(TSolutions phi)
        {
            // La matrice deve avere una colonna in più delle righe.
            // (La colonna dei termini noti.)
            static_assert(column_count() == row_count() + 1, "");
            constexpr auto n(TRowCount);

            // Tipo per gestire la precisione.
            using accuracy_type = double;

            // Precisione.
            accuracy_type accuracy(0.0001);

            // Divergenza massima corrente.
            accuracy_type max_divergence(0);

            // TODO:
            auto old_phi = phi;
            double sigma;

            do
            {
                // Inizialmente setta la divergenza massima come il minor
                // possibile valore rapprentabile.
                max_divergence = std::numeric_limits<accuracy_type>::lowest();

                // Itera sulle righe.
                for(int i = 0; i < n; ++i)
                {
                    // TODO:
                    sigma = 0.0;

                    // Itera sulle colonne.
                    for(int j = 0; j < n; ++j)
                    {
                        // Se `i == j`, salta l'iterazione.
                        if(i == j) continue;

                        sigma += (*this)(i, j) * phi[j];
                    }

                    phi[i] = (1.0 / ((*this)(i, i))) * ((*this)(i, n) - sigma);

                    // Calcola la divergenza tra la soluzione corrente e quella
                    // precedente.
                    auto divergence(std::fabs(phi[i] - old_phi[i]));

                    // Aggiorna la divergenza massima se necessario.
                    max_divergence = std::max(max_divergence, divergence);
                }

                // Sostituisce la soluzione precendente con quella corrente.
                old_phi = phi;
            } while(max_divergence > accuracy);

            // TODO:
            for(auto kk : phi) std::cout << kk << " | ";
            std::cout << "\n";

            // TODO: verificare divergenza -> no soluzioni
            return phi;
        }

        // TODO: usa il nostro vettore
        auto solve_gauss_seidel()
        {
            constexpr auto n(TRowCount);
            std::vector<double> phi(n, 0);
            return solve_gauss_seidel(phi);
        }
    };
}
