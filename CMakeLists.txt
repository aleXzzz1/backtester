cmake_minimum_required(VERSION 3.16)
project(csv_analyzer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.0  # check their releases page for current
)
FetchContent_MakeAvailable(Catch2)

add_executable(csv_analyzer src/main.cpp src/ohlcv.cpp src/csv_parser.cpp)


add_executable(tests
    tests/test_stats.cpp
    src/stats.cpp       # same source files, just linked into a different exe
    src/ohlcv.cpp
)

target_include_directories(tests PRIVATE src)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)