#include "solver.h"
// temp
#include <iostream>
namespace sas
{
    solver::solver(Incidence _incidence) : incidence(std::move(_incidence)), dimension(std::prev(_incidence.end())->first) {}
    void solver::get_degree_m()
    {
        std::cout << "dimension: " << dimension << std::endl;
        degree_m.resize(dimension, dimension);
        degree_m.reserve(dimension);
        std::cout << degree_m.rows() << ' ' << degree_m.cols() << std::endl;
        for (auto it : incidence)
        {
            std::cout << "degree: " << it.first << std::endl;
            if (!it.second.empty())
                degree_m.insert(it.first - 1, it.first - 1) = it.second.size();
        }
        degree_m.makeCompressed();
        std::cout << "degree_m:\n"
                  << degree_m << std::endl;
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
        // Make symmetric adjacency matrix from a directed graph
        adj_m = Eigen::SparseMatrix<int>(adj_m.transpose()).cwiseMax(adj_m);
        std::cout << "adj_m:\n"
                  << adj_m << std::endl;
    }
    std::vector<size_t> solver::allocation()
    {
        get_degree_m();
        get_adj_m();

        lap_m = degree_m - adj_m;
        // std::cout << lap_m.rows() << ' ' << lap_m.cols() << std::endl;
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

        // For disconnected graph
        // Number of extra sub graph, each of them is implied by 0 lambda, exclude the first one
        auto disconnected = (eigen_values.array() == 0).count() - 1;
        // First 0 lambda is the original trivial solution
        auto iter_zero = std::min_element(eigen_values.cbegin(), eigen_values.cend());
        while (0 < disconnected--)
        {
            std::cout << "extra disconnected: " << disconnected << std::endl;
            // Populate each disconnected graph
            iter_zero = std::min_element(++iter_zero, eigen_values.cend());
            // Corresponding eigen vector
            auto idx_zero_eigen_value = std::distance(eigen_values.cbegin(), iter_zero);
            Eigen::VectorXf zero_eigen_vector = eigensolver.eigenvectors().col(idx_zero_eigen_value);

            // Non zero enter for start of an independent graph
            auto lower_iter = std::find_if_not(zero_eigen_vector.cbegin(), zero_eigen_vector.cend(), [](auto p) { return p == 0; });
            auto lower_idx = std::distance(zero_eigen_vector.cbegin(), lower_iter);
            auto n = std::distance(lower_iter, zero_eigen_vector.cend());

            // Increase cost weight on each extra sub graph
            Eigen::VectorXf tmp = Eigen::VectorXf::Zero(dimension);
            tmp.segment(lower_idx, n).setOnes();
            eigen_vector2 += tmp;
        }

        fiedler_v = std::vector<float>(eigen_vector2.data(), eigen_vector2.data() + dimension); // Can use Eigen::vector

        std::vector<size_t> sort_idx(fiedler_v.size());
        std::iota(sort_idx.begin(), sort_idx.end(), 1); // May need to align with student input i.e. student 2 4 5 7
        auto comparator = [&](auto a, auto b) { return fiedler_v[a - 1] < fiedler_v[b - 1]; };
        std::sort(sort_idx.begin(), sort_idx.end(), comparator);
        sort_idx.resize(incidence.size()); // Only number of input students

        std::cout << "The eigenvalues of lap_m are:\n"
                  << eigensolver.eigenvalues() << std::endl;
        std::cout << "The eigenvalue2: " << idx_eigen_value2 << std::endl;
        std::cout << "Here's a matrix whose columns are eigenvectors of lap_m \n"
                  << "corresponding to these eigenvalues:\n"
                  << eigensolver.eigenvectors() << std::endl;
        std::cout << "fiedler_v:\n"
                  << eigen_vector2 << std::endl;

        return sort_idx;
    }
} // namespace sas