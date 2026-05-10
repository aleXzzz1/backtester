

struct Date {
    int year, month, day;
};

struct OHLCV {

    double m_open, m_high, m_low, m_close;
    std::int64_t m_volume;
    Date m_date;

    OHLCV(double open, double high, double low, double close, std::int64_t volume, Date date);

    double getOpen() const;
    
    double getHigh() const;

    double getLow() const;

    double getClose() const;

    int getVolume() const;

    Date getDate() const;

};