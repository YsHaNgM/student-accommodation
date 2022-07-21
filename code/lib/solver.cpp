#include "solver.h"
// temp
// #include <iostream>
namespace sas
{
    void solver::get_degree_m()
    {
        auto dimension = incidence.cend()->first;
        degree_m.resize(dimension, dimension);
        degree_m.reserve(dimension);
        for (auto it : incidence)
        {
            if (!it.second.empty())
                degree_m.insert(it.first - 1, it.first - 1) = it.second.size();
        }
        degree_m.makeCompressed();
    }
    void solver::tmp_get_m(Incidence _incidence)
    {
        incidence = std::move(_incidence);
        get_degree_m();
    }
} // namespace sas