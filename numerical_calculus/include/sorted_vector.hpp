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
    // Dettagli di implementazione.
    namespace impl
    {
        // Implemetazione del quicksort.
        template <typename T0>
        void sort_vector(T0* a, int n)
        {
            auto i(0);
            auto j(n - 1);

            if(n < 2) return;
            auto pivot(a[n / 2]);

            while(true)
            {
                while(a[i] < pivot) i++;
                while(pivot < a[j]) j--;

                if(i >= j) break;

                std::swap(a[i], a[j]);

                ++i;
                --j;
            }

            sort_vector(a, i);
            sort_vector(a + i, n - i);
        }

        // Ricerca binaria su un vettore `a` di lunghezza `len`.
        template <typename T0>
        auto binsearch(const T0* a, int len, const T0& x)
        {
            if(len == 0) return -1;
            int mid = len / 2;

            if(a[mid] == x) return mid;

            if(a[mid] < x)
            {
                auto result(binsearch(a + mid + 1, len - (mid + 1), x));

                if(result == -1) return -1;

                return result + mid + 1;
            }

            if(a[mid] > x) return binsearch(a, mid, x);
        }

        // Inserzione in ordine.
        template <typename T0>
        void insert_at(const T0& x, std::vector<T0>& vec)
        {
            auto old_size(vec.size());
            vec.resize(vec.size() + 1);

            int i(0);

            for(; i < old_size; ++i)
            {
                if(vec[i] < x) continue;

                for(int j(old_size); j > i; --j)
                {
                    vec[j] = vec[j - 1];
                }

                break;
            }

            vec[i] = x;
        }

        // Rimozione in ordine.
        template <typename T0>
        void remove_at(int idx, std::vector<T0>& vec)
        {
            for(; idx < vec.size() - 1; ++idx)
            {
                vec[idx] = vec[idx + 1];
            }

            vec.pop_back();
        }
    }

    // Interfaccia che chiama il quicksort su un vettore standard.
    template <typename T0>
    auto& sort_vector(std::vector<T0>& vec)
    {
        impl::sort_vector(vec.data(), vec.size());
        return vec;
    }

    // Interfaccia che chiama la ricerca binaria di un vettore standard.
    template <typename T0>
    auto find_in_sorted_vector(const T0& x, const std::vector<T0>& vec)
    {
        return impl::binsearch(vec.data(), vec.size(), x);
    }

    // Interfaccia che chiama l'inserzione ordinata in un vettore standard.
    template <typename T0>
    void insert_in_sorted_vector(const T0& x, std::vector<T0>& vec)
    {
        impl::insert_at(x, vec);
    }

    // Interfaccia che chiama la rimozione ordinata in un vettore standard.
    template <typename T0>
    void remove_from_sorted_vector(int idx, std::vector<T0>& vec)
    {
        impl::remove_at(idx, vec);
    }
}
