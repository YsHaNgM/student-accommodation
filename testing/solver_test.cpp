#include "solver.h"
#include "catch.hpp"
#include "catch_main.h"

TEST_CASE("Test solver", "[Solver]")
{
    SECTION("Test 4x4")
    {
        sas::Incidence student_set{
            {0, {1, 2, 3}},
            {1, {0, 3}},
            {2, {0, 3}},
            {3, {0, 1, 2}}};
        auto solver_ptr = std::make_unique<sas::solver>(student_set);
        auto student_order = solver_ptr->allocation();
        std::vector<size_t> solution{1, 0, 3, 2};
        Eigen::Matrix4i D;
        D << 3, 0, 0, 0,
            0, 2, 0, 0,
            0, 0, 2, 0,
            0, 0, 0, 3;
        Eigen::SparseMatrix<int> S = D.sparseView();
        REQUIRE(student_order == solution);
        REQUIRE(solver_ptr->get_degree_m().isApprox(S));
        D << 0, 1, 1, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 1, 1, 0;
        S = D.sparseView();
        REQUIRE(solver_ptr->get_adj_m().isApprox(S));
        D << 3, -1, -1, -1,
            -1, 2, 0, -1,
            -1, 0, 2, -1,
            -1, -1, -1, 3;
        REQUIRE(solver_ptr->get_lap_m().isApprox(D));
    }
    SECTION("Test 11x11 disconnected graph")
    {
        sas::Incidence student_set{
            {0, {1, 2, 3, 4}},
            {1, {0, 2}},
            {2, {0, 1}},
            {3, {0, 4}},
            {4, {0, 3}},
            {5, {6, 7}},
            {6, {5, 7}},
            {7, {5, 6}},
            {8, {}},
            {9, {10}},
            {10, {9}}};
        auto solver_ptr = std::make_unique<sas::solver>(student_set);
        auto student_order = solver_ptr->allocation();
        std::vector<size_t> solution{3, 4, 0, 2, 1, 5, 6, 7, 8, 9, 10};
        Eigen::Matrix<int, 11, 11> D;
        D << 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1;
        Eigen::SparseMatrix<int> S = D.sparseView();
        REQUIRE(student_order == solution);
        REQUIRE(solver_ptr->get_degree_m().isApprox(S));
        D << 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
            1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0;
        S = D.sparseView();
        REQUIRE(solver_ptr->get_adj_m().isApprox(S));
        D << 4, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0,
            -1, 2, -1, 0, 0, 0, 0, 0, 0, 0, 0,
            -1, -1, 2, 0, 0, 0, 0, 0, 0, 0, 0,
            -1, 0, 0, 2, -1, 0, 0, 0, 0, 0, 0,
            -1, 0, 0, -1, 2, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 2, -1, -1, 0, 0, 0,
            0, 0, 0, 0, 0, -1, 2, -1, 0, 0, 0,
            0, 0, 0, 0, 0, -1, -1, 2, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1,
            0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1;
        REQUIRE(solver_ptr->get_lap_m().isApprox(D));
    }
    SECTION("Test 4x4 directed graph")
    {
        sas::Incidence student_set{
            {0, {1, 3}},
            {1, {2}},
            {2, {0}},
            {3, {0}}};
        auto solver_ptr = std::make_unique<sas::solver>(student_set);
        auto student_order = solver_ptr->allocation();
        std::vector<size_t> solution{3, 0, 2, 1};
        Eigen::Matrix4i D;
        D << 2, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
        Eigen::SparseMatrix<int> S = D.sparseView();
        REQUIRE(student_order == solution);
        REQUIRE(solver_ptr->get_degree_m().isApprox(S));
        D << 0, 1, 1, 1,
            1, 0, 1, 0,
            1, 1, 0, 0,
            1, 0, 0, 0;
        S = D.sparseView();
        REQUIRE(solver_ptr->get_adj_m().isApprox(S));
        D << 2, -1, -1, -1,
            -1, 1, -1, 0,
            -1, -1, 1, 0,
            -1, 0, 0, 1;
        REQUIRE(solver_ptr->get_lap_m().isApprox(D));
    }
}