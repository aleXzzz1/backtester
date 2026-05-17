#ifndef MARKETEVENT_H
#define MARKETEVENT_H

#include <cstdint>
#include <string>
#include <chrono>

struct MarketEvent {
    std::string symbol_;
};

struct OHLCV : public MarketEvent {
    double open_, high_, low_, close_, volume_;
    std::chrono::year_month_day date_;

};


#endif