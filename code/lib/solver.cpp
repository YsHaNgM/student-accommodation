#include "solver.h"

namespace sas
{
    solver::solver(Incidence _incidence) : incidence(std::move(_incidence)), dimension(1 + std::prev(_incidence.end())->first) {}
    void solver::set_degree_m()
    {
        degree_m.resize(dimension, dimension);
        degree_m.reserve(dimension);
        for (auto it : incidence)
        {
            if (!it.second.empty())
                degree_m.insert(it.first, it.first) = it.second.size();
        }
        degree_m.makeCompressed();
    }
    void solver::set_adj_m()
    {
        adj_m.resize(dimension, dimension);
        auto entries = std::count_if(std::cbegin(incidence),
                                     std::cend(incidence),
                                     [](auto) { return true; });
        adj_m.reserve(entries);
        for (auto i : incidence)
        {
            for (auto j : i.second)
                adj_m.insert(i.first, j) = 1;
        }
        adj_m.makeCompressed();
        // Make symmetric adjacency matrix from a directed graph
        adj_m = Eigen::SparseMatrix<int>(adj_m.transpose()).cwiseMax(adj_m);
    }
    std::vector<size_t> solver::allocation()
    {
        set_degree_m();
        set_adj_m();

        lap_m = degree_m - adj_m;
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

        // Sort index regarding to entries of fiedler vector
        std::vector<size_t> sort_idx(fiedler_v.size());
        std::iota(sort_idx.begin(), sort_idx.end(), 0); // May need to align with student input i.e. student 2 4 5 7
        auto comparator = [&](auto a, auto b) { return fiedler_v[a] < fiedler_v[b]; };
        std::sort(sort_idx.begin(), sort_idx.end(), comparator);
        sort_idx.resize(incidence.size()); // Only number of input students

        return sort_idx;
    }
} // namespace sas