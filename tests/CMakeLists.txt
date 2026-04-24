
add_executable(
  unit_tests
  linux_parser_test.cpp
  format_test.cpp
)
target_link_libraries(unit_tests
PRIVATE
  monitor_lib
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(
  unit_tests
  )
  