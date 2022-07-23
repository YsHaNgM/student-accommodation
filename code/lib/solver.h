#ifndef solver_h
#define solver_h

#include <algorithm>
#include <numeric>
#include <Eigen>
#include "basic_types.h"

namespace sas
{
    class solver
    {
    public:
        solver(Incidence);
        ~solver() = default;
        std::vector<size_t> allocation();

    private:
        size_t dimension;
        Incidence incidence;
        Eigen::SparseMatrix<int> degree_m;
        Eigen::SparseMatrix<int> adj_m;
        Eigen::MatrixXi lap_m;

        void get_degree_m();
        void get_adj_m();
    };
} // namespace sas

#endif