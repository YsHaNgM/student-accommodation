#include "solver.h"
// temp
// #include <iostream>
namespace sas
{
    void solver::get_degree_m()
    {
        degree_m.resize(dimension, dimension);
        degree_m.reserve(dimension);
        for (auto it : incidence)
        {
            if (!it.second.empty())
                degree_m.insert(it.first - 1, it.first - 1) = it.second.size();
        }
        degree_m.makeCompressed();
    }
    void solver::get_adj_m()
    {
        adj_m.resize(dimension, dimension);
        auto entries = std::count_if(std::cbegin(incidence),
                                     std::cend(incidence),
                                     [](auto) { return true; });
        adj_m.reserve(entries);
        for (auto i : incidence)
        {
            for (auto j : i.second)
                adj_m.insert(i.first - 1, j - 1) = 1;
        }
        adj_m.makeCompressed();

        // std::cout << adj_m.rows() << ' ' << adj_m.cols() << std::endl;
        // std::cout << "Here is the matrix m:\n"
        //           << adj_m << std::endl;
    }
    std::vector<size_t> allocation()
    {
        ;
    }
    void solver::tmp_get_m(Incidence _incidence)
    {
        incidence = std::move(_incidence);
        dimension = incidence.cend()->first;
        get_degree_m();
        get_adj_m();
    }
} // namespace sas