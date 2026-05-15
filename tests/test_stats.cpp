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

std::vector<OHLCV> onlineClose = {
    OHLCV{0.0, 0.0, 0.0, 90.70, 0, d},
    OHLCV{0.0, 0.0, 0.0, 92.90, 0, d},
    OHLCV{0.0, 0.0, 0.0, 92.98, 0, d},
    OHLCV{0.0, 0.0, 0.0, 91.80, 0, d},
    OHLCV{0.0, 0.0, 0.0, 92.66, 0, d},
    OHLCV{0.0, 0.0, 0.0, 92.68, 0, d},
    OHLCV{0.0, 0.0, 0.0, 92.30, 0, d},
    OHLCV{0.0, 0.0, 0.0, 92.77, 0, d},
    OHLCV{0.0, 0.0, 0.0, 92.54, 0, d},
    OHLCV{0.0, 0.0, 0.0, 92.95, 0, d},
    OHLCV{0.0, 0.0, 0.0, 93.20, 0, d},
    OHLCV{0.0, 0.0, 0.0, 91.07, 0, d},
    OHLCV{0.0, 0.0, 0.0, 89.83, 0, d},
    OHLCV{0.0, 0.0, 0.0, 89.74, 0, d},
    OHLCV{0.0, 0.0, 0.0, 90.40, 0, d},
    OHLCV{0.0, 0.0, 0.0, 90.74, 0, d},
    OHLCV{0.0, 0.0, 0.0, 88.02, 0, d},
    OHLCV{0.0, 0.0, 0.0, 88.09, 0, d},
    OHLCV{0.0, 0.0, 0.0, 88.84, 0, d},
    OHLCV{0.0, 0.0, 0.0, 90.78, 0, d},
    OHLCV{0.0, 0.0, 0.0, 90.54, 0, d},
    OHLCV{0.0, 0.0, 0.0, 91.39, 0, d},
    OHLCV{0.0, 0.0, 0.0, 90.65, 0, d},
};

BollingerBand b_13 {};

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

TEST_CASE("stddev test") {

    std::deque<double> data {1.0, 2.0, 3.0, 4.0, 5.0};
    auto result = stddev(data, 3.0);
    REQUIRE(result == sqrt(2.0));
} 

TEST_CASE("BB of online price varying") {
    auto result = BollBands(onlineClose, 20);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(result[2] == std::nullopt);
    REQUIRE(result[3] == std::nullopt);
    REQUIRE(result[4] == std::nullopt);
    REQUIRE(result[5] == std::nullopt);
    REQUIRE(result[6] == std::nullopt);
    REQUIRE(result[7] == std::nullopt);
    REQUIRE(result[8] == std::nullopt);
    REQUIRE(result[9] == std::nullopt);
    REQUIRE(result[10] == std::nullopt);
    REQUIRE(result[11] == std::nullopt);
    REQUIRE(result[12] == std::nullopt);
    // REQUIRE(result[14]->lower == 87.12);
    // REQUIRE(result[14]->middle == 90.99);
    // REQUIRE(result[14]->upper == 94.86);
    // REQUIRE(result[16]->lower == );
    // REQUIRE(result[16]->middle == );
    // REQUIRE(result[16]->upper == );
    // EQUIRE(result[19]->lower == );
    // REQUIRE(result[19]->middle == );
    // REQUIRE(result[19]->upper == );
    REQUIRE(result[22]->lower == 87.95);
    REQUIRE(result[22]->middle == 91.05);
    REQUIRE(result[22]->upper == 94.15);
}