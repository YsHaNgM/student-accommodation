#include "solver.h"
// temp
#include <iostream>
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
    std::vector<size_t> solver::allocation()
    {
        lap_m = degree_m - adj_m;
        // std::cout << lap_m.rows() << ' ' << lap_m.cols() << std::endl;
        // std::cout << "Here is the matrix m:\n"
        //           << lap_m << std::endl;
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eigensolver(lap_m.cast<float>());
        if (eigensolver.info() != Eigen::Success)
            abort();
        std::cout << "The eigenvalues of lap_m are:\n"
                  << eigensolver.eigenvalues() << std::endl;
        std::cout << "Here's a matrix whose columns are eigenvectors of lap_m \n"
                  << "corresponding to these eigenvalues:\n"
                  << eigensolver.eigenvectors() << std::endl;
        return std::vector<size_t>(0);
    }
    void solver::tmp_get_m(Incidence _incidence)
    {
        incidence = std::move(_incidence);
        dimension = incidence.cend()->first;
        get_degree_m();
        get_adj_m();
        allocation();
    }
} // namespace sas