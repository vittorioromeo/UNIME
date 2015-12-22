
    template <typename T0, std::size_t TDim>
    auto calc_u_ij(
        const matrix<T0, TDim, TDim>& a, std::size_t i, std::size_t j)
    {
        auto sum = 0;

        if(i > 0)
            for(auto k(0); k < i - 1; ++k)
            {
                sum += a(i, k) * a(k, j);
            }


        return a(i, j) - sum;
    }

    template <typename T0, std::size_t TDim>
    auto calc_l_ij(
        const matrix<T0, TDim, TDim>& a, std::size_t i, std::size_t j)
    {
        auto sum = 0;

        if(j > 0)
            for(auto k(0); k < j - 1; ++k)
            {
                sum += (a(i, k) * a(k, j));
            }

        return (a(i, j) - sum) / a(j, j);
    }

    template <typename T0, std::size_t TDim>
    auto calc_lu_row(
        matrix<T0, TDim, TDim>& a, std::size_t fixed_i, std::size_t offset)
    {
        for(std::size_t j(offset); j < TDim; ++j)
        {
            // std::cout << "a(" << fixed_i << ", " << j
            //          << ") = " << calc_u_ij(a, fixed_i, j) << "\n";

            a(fixed_i, j) = calc_u_ij(a, fixed_i, j);
        }
    }

    template <typename T0, std::size_t TDim>
    auto calc_lu_col(
        matrix<T0, TDim, TDim>& a, std::size_t fixed_j, std::size_t offset)
    {
        for(std::size_t i(offset); i < TDim; ++i)
        {
            // std::cout << "a(" << i << ", " << fixed_j
            //           << ") = " << calc_l_ij(a, i, fixed_j) << "\n";

            a(i, fixed_j) = calc_l_ij(a, i, fixed_j);
        }
    }

    // TODO: fix using data
    template <typename T0, std::size_t TDim>
    auto make_ul_decomposition(matrix<T0, TDim, TDim> m)
    {
        // matrix<T0, TDim, TDim> u, l;
        // u = m;
        //        matrix<T0, TDim, TDim> r = m;

        auto row_start(0);
        auto row_offset(0);
        auto col_start(0);
        auto col_offset(1);

        while(true)
        {
            if(row_start < TDim && row_offset < TDim)
            {
                /*
                                std::cout << "\n";

                                std::cout << "calc_lu_row:\n";
                                std::cout << "rs=" << row_start << ", ro=" <<
                   row_offset
                                          << "\n";
                */
                calc_lu_row(m, row_start, row_offset);
            }
            else
                break;

            ++row_start;
            ++row_offset;

            if(col_start < TDim && col_offset < TDim)
            {
                /*
                                std::cout << "\n";
                                std::cout << "calc_lu_col:\n";
                                std::cout << "cs=" << col_start << ", co=" <<
                   col_offset
                                          << "\n";
                */
                calc_lu_col(m, col_start, col_offset);
            }
            else
                break;

            ++col_start;
            ++col_offset;
        }

        return m;
    }