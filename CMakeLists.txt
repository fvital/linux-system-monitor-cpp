
cmake_minimum_required(VERSION 3.24)
project(monitor CXX)


# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
