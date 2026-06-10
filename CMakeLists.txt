cmake_minimum_required(VERSION 3.16)
project(backtester CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -g -O3)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.0  # check their releases page for current
)
FetchContent_MakeAvailable(Catch2)

include(FetchContent)
FetchContent_Declare(
    fast_float
    GIT_REPOSITORY https://github.com/fastfloat/fast_float.git
    GIT_TAG v8.2.6   # pin a real tag; check the repo for the latest
)
FetchContent_MakeAvailable(fast_float)

# --- Shared sources (everything except main.cpp and tests) ---
set(BACKTESTER_SOURCES
    src/Engine.cpp
    src/ConsoleUI.cpp
    src/data/EagerCSVFeed.cpp
    src/data/LazyCSVFeed.cpp
    src/market/MarketContext.cpp
    src/portfolio/Portfolio.cpp
    src/execution/Broker.cpp
    src/strategy/MAC.cpp
    src/strategy/Indicators.cpp
    src/strategy/Momentum.cpp
    src/analysis/Analytics.cpp
    
)

set(BACKTESTER_INCLUDE_DIRS
    src
    src/data
    src/market
    src/portfolio
    src/execution
    src/strategy
    src/analysis
)

# --- Static library so backtester and tests share compiled code ---
add_library(backtester_lib STATIC ${BACKTESTER_SOURCES})
target_include_directories(backtester_lib PUBLIC ${BACKTESTER_INCLUDE_DIRS})

# --- Main executable ---
add_executable(backtester src/main.cpp)
target_link_libraries(backtester PRIVATE backtester_lib)
target_link_libraries(backtester_lib PUBLIC fast_float)

# --- Test executable ---
add_executable(test_analytics tests/test_analytics.cpp)
target_link_libraries(test_analytics PRIVATE backtester_lib Catch2::Catch2WithMain)
target_include_directories(test_analytics PRIVATE src src/analysis)