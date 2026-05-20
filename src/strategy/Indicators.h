#ifndef INDICATORS_H
#define INDICATORS_H

#include <vector>
#include "MarketEvent.h"
#include <iostream>
#include <fstream>
#include <optional>
#include <deque>

struct BollingerBand {
    double upper;
    double middle;
    double lower;
};

class SMA {
    public:
    explicit SMA(int period): period_(period) {}

    std::optional<double> update(double price);

    std::optional<double> value() const;
    
    private:
    int period_;
    double sum_ {0.0};
    std::deque<double> window_;
};

std::vector<std::optional<double>> closing_SMA(const std::vector<OHLCV>& prices, int window);

std::vector<std::optional<double>> closing_EMA(const std::vector<OHLCV>& prices, int window);

std::vector<std::optional<BollingerBand>> BollBands(const std::vector<OHLCV>& prices, int window);

double stddev(const std::deque<double>& vals, double sma);

#endif