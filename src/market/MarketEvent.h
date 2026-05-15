#ifndef MARKETEVENT_H
#define MARKETEVENT_H

#include <cstdint>
#include <string>
#include <chrono>

class MarketEvent {

};

struct OHLCV : public MarketEvent {
    double open_, high_, low_, close_;
    std::int64_t volume_;
    std::chrono::year_month_day date_;

};


#endif