
add_executable(
  unit_tests
  linux_parser_test.cpp
  meminfo_test.cpp
  format_test.cpp
  stat_test.cpp
)
target_link_libraries(unit_tests
PRIVATE
  monitor_lib
  GTest::gtest_main
)

target_compile_definitions(unit_tests
  PRIVATE
    TEST_FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fixtures"
    )

include(GoogleTest)
gtest_discover_tests(
  unit_tests
  )
  