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

inline double execution_price(const OHLCV& bar) { return bar.open_; }   // bar fills happen at next open
inline double mark_price(const OHLCV& bar)      { return bar.close_; }  // value positions at close
inline auto   event_time(const OHLCV& bar)      { return std::chrono::sys_days(bar.date_); }


#endif