#pragma once

#include <limits>
#include <cmath>
#include <type_traits>

namespace nc
{
    // Tipo per gestire la precisione.
    using accuracy_type = double;

    // Itera su una variabile, saltando un valore.
    template <typename TItr0, typename TItr1, typename TItr2, typename TF>
    auto loop_skipping(TItr0 k_min, TItr1 k_max, TItr2 k_skip, TF fn)
    {
        for(auto k(k_min); k < k_skip; ++k)
        {
            fn(k);
        }
        // Saltiamo l'indice desiderato.
        for(auto k(k_skip + 1); k < k_max; ++k)
        {
            fn(k);
        }
    };

    // Itera controllando la divergenza e le iterazioni massime.
    template <typename TF>
    auto divergence_loop(
        std::size_t max_iterations, accuracy_type accuracy, TF fn_body)
    {
        // Divergenza massima corrente.
        accuracy_type max_divergence(0);

        // Ripeti al massimo `max_iterations` iterazioni.
        for(std::size_t itr(0); itr < max_iterations; ++itr)
        {
            // Inizialmente setta la divergenza massima come il minor
            // possibile valore rapprentabile.
            max_divergence = std::numeric_limits<accuracy_type>::lowest();

            // Esegui il corpo del loop, che aggiorna `max_divergence` se
            // necessario.
            fn_body(max_divergence);

            // Continua ad iterare finchè tutte le divergenze sono maggiori
            // di `accuracy`.
            if(max_divergence <= accuracy)
            {
                break;
            }
        };
    }

    // Aggiorna la divergenza massima se necessario.
    template <typename TItr, typename TVec0, typename TVec1, typename TMaxDiv>
    auto update_max_divergence(TItr i, const TVec0& temp_vec, const TVec1& vec,
        TMaxDiv& max_divergence)
    {
        using temp_v_t = std::decay_t<decltype(temp_vec(0, 0))>;
        using v_t = std::decay_t<decltype(vec(0, 0))>;

        // Tipo in comune tra i vettori.
        using common = std::common_type_t<temp_v_t, v_t>;

        // Calcola la divergenza tra la soluzione corrente e
        // quella precedente.
        auto divergence(std::fabs((common)vec(i, 0) - (common)temp_vec(i, 0)));

        // Aggiorna la divergenza massima se necessario.
        max_divergence = std::max((common)max_divergence, (common)divergence);
    };

    // Esegue `f` per ogni argomento `xs...`.
    template <typename TF, typename... Ts>
    void for_args(TF&& f, Ts&&... xs)
    {
        using intarray = int[];
        (void)intarray{(f(xs), 0)...};
    }
}
