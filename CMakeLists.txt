cmake_minimum_required(VERSION 3.16)
project(backtester CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.0  # check their releases page for current
)
FetchContent_MakeAvailable(Catch2)

# --- Shared sources (everything except main.cpp and tests) ---
set(BACKTESTER_SOURCES
    src/Engine.cpp
    src/data/EagerCSVFeed.cpp
    src/market/MarketContext.cpp
    src/portfolio/Portfolio.cpp
    src/execution/Broker.cpp
    src/strategy/MAC.cpp
    src/strategy/Indicators.cpp
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