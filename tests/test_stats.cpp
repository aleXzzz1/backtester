#include <catch2/catch_test_macros.hpp>
#include "stats.h"     // the header of what you're testing
#include "ohlcv.h"     // because you'll need to construct OHLCV inputs
Date d{2020,01,01};
std::vector<OHLCV> increasingClose = {
        OHLCV{0.0, 0.0, 0.0, 1.0, 0, d}, 
        OHLCV{0.0, 0.0, 0.0, 2.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 3.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 4.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
    };

std::vector<OHLCV> randomClose = {
        OHLCV{0.0, 0.0, 0.0, 2.0, 0, d}, 
        OHLCV{0.0, 0.0, 0.0, 4.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 7.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 3.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 2.0, 0, d},
    };

std::vector<OHLCV> constantClose = {
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d}, 
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
    };

TEST_CASE("SMA of increasing close price", "[sma]") {
    auto result = closing_SMA(increasingClose, 3);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(*result[2] == 2.0);
    REQUIRE(*result[3] == 3.0);
    REQUIRE(*result[4] == 4.0);
}

TEST_CASE("SMA of constant closing prices equals that constant", "[sma]") {
    auto result = closing_SMA(constantClose, 3);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(*result[2] == 5.0);
    REQUIRE(*result[3] == 5.0);
    REQUIRE(*result[4] == 5.0);
}

TEST_CASE("SMA of 1 day period should be input close values", "[sma]") {
    auto result = closing_SMA(randomClose, 1);
    REQUIRE(*result[0] == 2.0);
    REQUIRE(*result[1] == 4.0);
    REQUIRE(*result[2] == 7.0);
    REQUIRE(*result[3] == 3.0);
    REQUIRE(*result[4] == 2.0);
}

TEST_CASE("SMA of constant closing prices for 5 day period", "[sma]") {
    auto result = closing_SMA(constantClose, 5);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(result[2] == std::nullopt);
    REQUIRE(result[3] == std::nullopt);
    REQUIRE(*result[4] == 5.0);
}

TEST_CASE("EMA of constant closing prices for 5 day period", "[ema]") {
    auto result = closing_EMA(constantClose, 5);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(result[2] == std::nullopt);
    REQUIRE(result[3] == std::nullopt);
    REQUIRE(*result[4] == 5.0);

}

TEST_CASE("EMA of increasing closing price for 3 day period") {
    auto result = closing_EMA(constantClose, 3);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(*result[2] == 5.0);
    REQUIRE(*result[3] == 5.0);
    REQUIRE(*result[4] == 5.0);
}

TEST_CASE("EMA of varying closing price for 3 day period") {
    double n = (1.5+(13.0/3.0)*(0.5));
    auto result = closing_EMA(randomClose, 3);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(*result[2] == 13.0/3.0);
    REQUIRE(*result[3] == (1.5+(13.0/3.0)*(0.5)));
    REQUIRE(*result[4] == (1+n*(0.5)));
}