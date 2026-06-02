#include <catch2/catch_test_macros.hpp>
#include "Analytics.h"  // because you'll need to construct OHLCV inputs
#include "Event.h"

static timestamp day(int n) {
    return std::chrono::sys_days{std::chrono::January / 1 / 2020} + std::chrono::days{n};
}

TEST_CASE("total return: positive") {
    std::vector<EquityPoint> eqcurve = {
        {.ts = day(0), .equity = 100.0},
        {.ts = day(1), .equity = 90.0},
        {.ts = day(2), .equity = 85.0},
        {.ts = day(3), .equity = 104.0},
        {.ts = day(4), .equity = 50.0},
        {.ts = day(5), .equity = 106.0},
    };
    auto result = Analytics::total_return(eqcurve);
    REQUIRE(result == ((106.0 / 100.0) - 1.0));
}

TEST_CASE("total return: negative") {
    std::vector<EquityPoint> eqcurve = {
        {.ts = day(0), .equity = 100.0},
        {.ts = day(1), .equity = 90.0},
        {.ts = day(2), .equity = 85.0},
        {.ts = day(3), .equity = 104.0},
        {.ts = day(4), .equity = 50.0},
        {.ts = day(5), .equity = 50.0},
    };
    auto result = Analytics::total_return(eqcurve);
    REQUIRE(result == (-0.5));
}

TEST_CASE("max drawdown") {
    std::vector<EquityPoint> eqcurve = {
        {.ts = day(0), .equity = 100.0},
        {.ts = day(1), .equity = 90.0},
        {.ts = day(2), .equity = 85.0},
        {.ts = day(3), .equity = 104.0},
        {.ts = day(4), .equity = 50.0},
        {.ts = day(5), .equity = 106.0},
    };
    auto result = Analytics::max_drawdown(eqcurve);
    REQUIRE(result == ((50.0 - 104.0) / 104.0));
}

TEST_CASE("max drawdown: strictly increasing") {
    std::vector<EquityPoint> eqcurve = {
        {.ts = day(0), .equity = 100.0},
        {.ts = day(1), .equity = 101.0},
        {.ts = day(2), .equity = 102.0},
        {.ts = day(3), .equity = 102.5},
        {.ts = day(4), .equity = 103.0},
        {.ts = day(5), .equity = 106.0},
    };
    auto result = Analytics::max_drawdown(eqcurve);
    REQUIRE(result == 0.0);
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