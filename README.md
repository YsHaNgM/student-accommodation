# student-accommodation
Graph optimisation problem - spectral clustering, find the allocation of vertices (i.e. students) to minimise the total length of edges that represent connection routes between vertices. The allocation in this problem is in a 1-dimension array space. The solver uses Fiedler ordering which sorts the allocation of vertices using Fiedler vector of Laplacian matrix.

[Laplacian matrix](https://en.wikipedia.org/wiki/Laplacian_matrix) is `L = D - A` by defining degree matrix `D` and adjacency matrix `A`. Fiedler vector is the eigen vector corresponding to the smallest non-zero eigen value of Laplacian matrix.

## Build Instructions

[GCC 9.4](https://gcc.gnu.org/gcc-9/), [Clang 10](http://releases.llvm.org/10.0.0/tools/clang/docs/) or newer version needs to be used.


```
mkdir build
cd build
cmake ../
make
```

Build in debug then use `ctest` to run testing cases.

## Use Instructions

Run command line app sas_app under build/bin/ with a file provides students friendship set. The content could be:
```
1 2 3 4
2 1 4
3 1 4
4 1 2 3
```
Where the first number in each line represents student index, rest of the line gives corresponding friends of him, which the student will visit during a day. The output is allocation of students.

## Improvements to-do

- Solver uses the Eigen dense matrix at the moment, it can choose appropriate Sparse linear algebra to optimase.