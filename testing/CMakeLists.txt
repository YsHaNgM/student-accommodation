set(DATA_DIR ${CMAKE_SOURCE_DIR}/testing/data)
set(TMP_DIR ${CMAKE_BINARY_DIR}/testing/temporary)

set(TEST_CASES
  solver_test
)

foreach(_test_case ${TEST_CASES})
  add_executable(${_test_case} ${_test_case}.cpp catch_main.cpp)
  target_link_libraries(${_test_case} ${ALL_THIRD_PARTY_LIBRARIES} sas)
endforeach()

# Adding tests is done outside the above loop,
# as each test will need different arguments.

##################################################################################################
# Dont forget its:  add_test(<test name (globally unique) > <exe name> <argument1> <argument2>
##################################################################################################

add_test(solver ${EXECUTABLE_OUTPUT_PATH}/solver_test)

