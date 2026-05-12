cmake_minimum_required(VERSION 3.16)
project(csv_analyzer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(csv_analyzer src/main.cpp src/ohlcv.cpp src/csv_parser.cpp)