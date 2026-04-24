
add_library(monitor_lib 
    format.cpp
    linux_parser.cpp 
    ncurses_display.cpp 
    process.cpp 
    processor.cpp 
    system.cpp 
    meminfo.cpp
)

target_include_directories(monitor_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(monitor_lib ${CURSES_LIBRARIES})

add_executable(monitor main.cpp)

target_link_libraries(monitor PRIVATE monitor_lib)
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
