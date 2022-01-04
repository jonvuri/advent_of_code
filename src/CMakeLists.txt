cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

find_package(fmt)
find_package(spdlog REQUIRED)
find_package(docopt)
find_package(doctest REQUIRED)

include(doctest)
include(CTest)

set (SOLVER_FILES
  01/part1/solver_01_part1.cpp
  01/part2/solver_01_part2.cpp
  02/part1/solver_02_part1.cpp
  02/part2/solver_02_part2.cpp
  03/part1/solver_03_part1.cpp
  03/part2/solver_03_part2.cpp
  04/part1/solver_04_part1.cpp
  04/part2/solver_04_part2.cpp
  05/part1/solver_05_part1.cpp
  05/part2/solver_05_part2.cpp
  06/part1/solver_06_part1.cpp
  06/part2/solver_06_part2.cpp
  07/part1/solver_07_part1.cpp
  07/part2/solver_07_part2.cpp
  08/part1/solver_08_part1.cpp
  08/part2/solver_08_part2.cpp
  09/part1/solver_09_part1.cpp
)

# Make main executable
add_executable(
  ${MAIN}
  main.cpp
  ${SOLVER_FILES})

target_link_libraries(
  ${MAIN}
  PRIVATE project_options
          project_warnings
          docopt::docopt
          doctest::doctest
          spdlog::spdlog)
          
set_target_properties(${MAIN} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_definitions(${MAIN} PRIVATE DOCTEST_CONFIG_DISABLE=1)

# Parameters
set(TESTFILES        # All .cpp files in tests/
    tests.cpp
    ${SOLVER_FILES}
)
set(TEST_MAIN unit_tests)
set(TEST_RUNNER_PARAMS "")

# Make unit test executable
add_executable(${TEST_MAIN} ${TESTFILES})
target_link_libraries(${TEST_MAIN} PRIVATE doctest::doctest spdlog::spdlog)
set_target_properties(${TEST_MAIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set_target_properties(${TEST_MAIN} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Add tests to CTest
doctest_discover_tests(
  ${TEST_MAIN}
  TEST_PREFIX advent
  ADD_LABELS 1
)

