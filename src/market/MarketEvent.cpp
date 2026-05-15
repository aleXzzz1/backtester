#include <cstdint>
#include "MarketEvent.h"


std::string toString(Date d) {
    std::string s;
    s+= std::to_string(d.year);
    s+= '-';
    s+= std::to_string(d.month);
    s+='-';
    s+= std::to_string(d.day);
    return s;
}

std::string toString(const OHLCV& bar) {
    std::string s;
    s += ": open=";   s += std::to_string(bar.m_open);
    s += " high=";    s += std::to_string(bar.m_high);
    s += " low=";     s += std::to_string(bar.m_low);
    s += " close=";   s += std::to_string(bar.m_close);
    s += " volume=";  s += std::to_string(bar.m_volume);
    return s;
}
