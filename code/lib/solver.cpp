#include "solver.h"
// temp
#include <iostream>
namespace sas
{
    solver::solver(Incidence _incidence) : incidence(std::move(_incidence)), dimension(_incidence.cend()->first) {}
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
    }
    std::vector<size_t> solver::allocation()
    {
        get_degree_m();
        get_adj_m();

        lap_m = degree_m - adj_m;
        std::cout << lap_m.rows() << ' ' << lap_m.cols() << std::endl;
        std::cout << "Here is the matrix m:\n"
                  << lap_m << std::endl;
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eigensolver(lap_m.cast<float>());
        if (eigensolver.info() != Eigen::Success)
            abort();
        auto eigen_values = eigensolver.eigenvalues();
        // simple graph laplacian, non negative, smallest always 0
        eigen_values = (eigen_values.array() < 0).select(0, eigen_values);
        auto iter = std::min_element(eigen_values.cbegin(), eigen_values.cend(), [](auto x, auto y) {
            return (x < y && x > 0) || y == 0;
        });

        std::vector<float> fiedler_v;
        auto idx_eigen_value2 = std::distance(eigen_values.cbegin(), iter);
        Eigen::VectorXf eigen_vector2 = eigensolver.eigenvectors().col(idx_eigen_value2);
        //temp for disconnected graph
        if (auto disconnected = (eigen_values.array() == 0).count() - 1)
        {
            //disconnected+=1 populate each disconnected graph should be a loop
            auto iter_zero = std::min_element(eigen_values.cbegin(), eigen_values.cend());
            auto idx_eigen_value1 = std::distance(eigen_values.cbegin(), iter_zero);
            Eigen::VectorXf eigen_vector1 = eigensolver.eigenvectors().col(idx_eigen_value1);

            auto lower_iter = std::find(eigen_vector1.cbegin(), eigen_vector1.cend(), 0);
            auto lower_idx = std::distance(eigen_vector1.cbegin(), lower_iter);
            auto upper_iter = std::find_if_not(lower_iter, eigen_vector1.cend(), [](auto p) { return p == 0; });
            auto n = std::distance(lower_iter, upper_iter);

            Eigen::VectorXf v = Eigen::VectorXf::Zero(dimension);
            v.segment(lower_idx, n).setOnes();
            std::cout << "v: " << std::endl
                      << v << std::endl;

            eigen_vector2 = eigen_vector2 + v;
        }

        fiedler_v = std::vector<float>(eigen_vector2.data(), eigen_vector2.data() + dimension);

        std::vector<size_t> sort_idx(fiedler_v.size());
        std::iota(sort_idx.begin(), sort_idx.end(), 1);
        auto comparator = [&](auto a, auto b) { return fiedler_v[a - 1] < fiedler_v[b - 1]; };
        std::sort(sort_idx.begin(), sort_idx.end(), comparator);

        std::cout << "The eigenvalues of lap_m are:\n"
                  << eigensolver.eigenvalues() << std::endl;
        std::cout << "The eigenvalue2: " << idx_eigen_value2 << std::endl;
        for (auto v : sort_idx)
            std::cout << v << ' ';
        std::cout << std::endl;
        std::cout << "Here's a matrix whose columns are eigenvectors of lap_m \n"
                  << "corresponding to these eigenvalues:\n"
                  << eigensolver.eigenvectors() << std::endl;
        std::cout << "degree_m:\n"
                  << degree_m << std::endl;
        std::cout << "adj_m:\n"
                  << adj_m << std::endl;
        std::cout << "fiedler_v:\n"
                  << eigen_vector2 << std::endl;

        return sort_idx;
    }
} // namespace sas