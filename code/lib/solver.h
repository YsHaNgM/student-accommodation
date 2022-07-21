#ifndef solver_h
#define solver_h

#include <algorithm>
#include <Eigen>
#include "basic_types.h"

namespace sas
{
    class solver
    {
    public:
        solver() = default;
        ~solver() = default;
        void tmp_get_m(Incidence);
        std::vector<size_t> allocation();

    private:
        size_t dimension;
        Incidence incidence;
        Eigen::SparseMatrix<int> degree_m;
        Eigen::SparseMatrix<int> adj_m;
        Eigen::MatrixXi lap_m;
        Eigen::VectorXf fiedler_v;

        void get_degree_m();
        void get_adj_m();
    };
} // namespace sas

#endif