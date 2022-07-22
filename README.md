# student-accommodation
graph optimisation problem, find the allocation of vertices (i.e. students) to minimise the total length of edges that represent connections routes between vertices. The allocation in this problem is in a 1-dimension array space.

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

Run command line app sas_app under build/bin/.
