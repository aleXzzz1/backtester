#include "ohlcv.h"


OHLCV::OHLCV(double open, double high, double low, double close, std::int64_t volume, Date date) 
: m_open(open), m_high(high), m_low(low), m_close(close), m_volume(volume), m_date(date) {

}

double OHLCV::getOpen() const {
    return m_open;
}

double OHLCV::getHigh() const {
    return m_high;
}

double OHLCV::getLow() const {
    return m_low;
}

double OHLCV::getClose() const {
    return m_close;
}

int OHLCV::getVolume() const {
    return m_volume;
}

Date OHLCV::getDate() const {
    return m_date;
}
