#include <catch2/catch_test_macros.hpp>
#include "Analytics.h"  // because you'll need to construct OHLCV inputs
#include "Event.h"

static timestamp day(int n) {
    return std::chrono::sys_days{std::chrono::January / 1 / 2020} + std::chrono::days{n};
}

TEST_CASE("max drawdown days") {
    std::vector<EquityPoint> eqcurve = {
        {.ts = day(0), .equity = 100.0},
        {.ts = day(1), .equity = 90.0},
        {.ts = day(2), .equity = 99.0},
        {.ts = day(3), .equity = 104.0},
    };
    auto result = Analytics::max_drawdown_days(eqcurve);
    REQUIRE(result == 3);
}

TEST_CASE("max drawdown days: strictly increasing") {
    std::vector<EquityPoint> eqcurve = {
        {.ts = day(0), .equity = 100.0},
        {.ts = day(1), .equity = 101.0},
        {.ts = day(2), .equity = 102.0},
        {.ts = day(3), .equity = 104.0},
    };
    auto result = Analytics::max_drawdown_days(eqcurve);
    REQUIRE(result == 0);
}

/*
TEST_CASE("EMA of varying closing price for 3 day period") {
    double n = (1.5+(13.0/3.0)*(0.5));
    auto result = closing_EMA(randomClose, 3);
    REQUIRE(result[0] == std::nullopt);
    REQUIRE(result[1] == std::nullopt);
    REQUIRE(*result[2] == 13.0/3.0);
    REQUIRE(*result[3] == (1.5+(13.0/3.0)*(0.5)));
    REQUIRE(*result[4] == (1+n*(0.5)));
}
*/