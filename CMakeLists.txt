######################################################################
# Set the minimum CMake version.
######################################################################
# CMake 3.16 is needed for Apple clang
cmake_minimum_required(VERSION 3.16) 
project(student_accommodation_sorting)

######################################################################
# Set main build options.
######################################################################
option(BUILD_TESTING "Build Unit tests." ON)
option(USE_OPENMP "Enable OpenMP" OFF)
option(USE_MPI "Enable MPI" OFF)

######################################################################
# Choose C++ standard and set language property. 
######################################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

######################################################################
# Setting supported build types. Should ONLY be Release or Debug.
######################################################################
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Valid options are Release or Debug" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug")
endif()

if (NOT (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Debug"))
  message(FATAL_ERROR "Build type \"${CMAKE_BUILD_TYPE}\" is not supported.")
endif()

######################################################################
# Print out where the source and binary folders
# are, just to make it really explicit.
######################################################################
message("CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

######################################################################
# OMP and MPI staff.
######################################################################
if(USE_OPENMP)
    find_package(OpenMP REQUIRED)
    list(APPEND ALL_THIRD_PARTY_LIBRARIES PUBLIC OpenMP::OpenMP_CXX)
else()
    if( (CMAKE_CXX_COMPILER_ID MATCHES "[cC][lL][aA][nN][gG]")
            OR (CMAKE_CXX_COMPILER_ID MATCHES "[gG][nN][uU]"))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
    endif()
endif(USE_OPENMP)


if(USE_MPI)
  find_package(MPI REQUIRED)
  message("Found MPI, with mpiexec=${MPIEXEC}")
  include_directories(${MPI_CXX_INCLUDE_PATH})
  set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} ${MPI_CXX_COMPILE_FLAGS} ")
  set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS} ${MPI_CXX_LINK_FLAGS} ")
  add_compile_definitions(_MPI ${USE_MPI})
  list(APPEND ALL_THIRD_PARTY_LIBRARIES ${MPI_CXX_LIBRARIES})

  message("Adding MPI include:${MPI_CXX_INCLUDE_PATH}.")
  message("Adding MPI libs:${MPI_CXX_LIBRARIES}.")

endif()

######################################################################
# Set up a few paths.
######################################################################
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(SAS_INSTALL_LIB_DIR lib)
set(SAS_INSTALL_INC_DIR include)
set(SAS_INSTALL_BIN_DIR bin)

foreach(type LIBRARY RUNTIME ARCHIVE)
  set(output_dir ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_${type}_OUTPUT_DIRECTORY ${output_dir} CACHE INTERNAL "Single output directory for building all libraries.")
  mark_as_advanced(CMAKE_${type}_OUTPUT_DIRECTORY)
endforeach()

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
include_directories(${CMAKE_SOURCE_DIR}/code/3rd_party/eigen-3.4.0/Eigen)
include_directories(${CMAKE_SOURCE_DIR}/code/lib)
include_directories(${CMAKE_BINARY_DIR})

######################################################################
# Add our main code folders. This is where all our functionality is.
######################################################################
add_subdirectory(code)
if(BUILD_TESTING)
  enable_testing ()
  set(TEMP_DIR ${CMAKE_BINARY_DIR}/testing/temporary)
  include_directories(${CMAKE_SOURCE_DIR}/testing/)
  add_subdirectory(testing)
endif()