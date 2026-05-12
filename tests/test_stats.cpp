#include <catch2/catch_test_macros.hpp>
#include "stats.h"     // the header of what you're testing
#include "ohlcv.h"     // because you'll need to construct OHLCV inputs

TEST_CASE("SMA of varying close price", "[sma]") {
    Date d{2020,01,01};
    std::vector<OHLCV> data = {
        OHLCV{0.0, 0.0, 0.0, 1.0, 0, d}, 
        OHLCV{0.0, 0.0, 0.0, 2.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 3.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 4.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
    };
    auto result = closing_SMA(data, 3);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(*result[2] == 2.0);
    REQUIRE(*result[3] == 3.0);
    REQUIRE(*result[4] == 4.0);
}

TEST_CASE("SMA of constant closing prices equals that constant", "[sma]") {
    Date d{2020,01,01};
    std::vector<OHLCV> data = {
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d}, 
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
    };
    auto result = closing_SMA(data, 3);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(*result[2] == 5.0);
    REQUIRE(*result[3] == 5.0);
    REQUIRE(*result[4] == 5.0);
}

TEST_CASE("SMA of 1 day period should be input close values", "[sma]") {
    Date d{2020,01,01};
    std::vector<OHLCV> data = {
        OHLCV{0.0, 0.0, 0.0, 2.0, 0, d}, 
        OHLCV{0.0, 0.0, 0.0, 4.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 7.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 3.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 2.0, 0, d},
    };
    auto result = closing_SMA(data, 1);
    REQUIRE(*result[0] == 2.0);
    REQUIRE(*result[1] == 4.0);
    REQUIRE(*result[2] == 7.0);
    REQUIRE(*result[3] == 3.0);
    REQUIRE(*result[4] == 2.0);
}

TEST_CASE("SMA of constant closing prices for 5 day period", "[sma]") {
    Date d{2020,01,01};
    std::vector<OHLCV> data = {
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d}, 
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
        OHLCV{0.0, 0.0, 0.0, 5.0, 0, d},
    };
    auto result = closing_SMA(data, 5);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(result[2] == std::nullopt);
    REQUIRE(result[3] == std::nullopt);
    REQUIRE(*result[4] == 5.0);
}