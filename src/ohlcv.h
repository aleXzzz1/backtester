#ifndef OHLCV_H
#define OHLCV_H

#include <cstdint>
#include <string>

struct Date {
    int year, month, day;
};

struct OHLCV {
    double m_open, m_high, m_low, m_close;
    std::int64_t m_volume;
    Date m_date;

};

std::string toString(Date d);
std::string toString(const OHLCV& bar);


#endif