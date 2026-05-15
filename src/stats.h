#ifndef STATS_H
#define STATS_H

#include <vector>
#include "ohlcv.h"
#include <iostream>
#include <fstream>
#include <optional>

struct BollingerBand {
    double upper;
    double middle;
    double lower;
};

void buildCSV(const std::vector<OHLCV>& vec);

std::vector<std::optional<double>> closing_SMA(const std::vector<OHLCV>& prices, int window);

std::vector<std::optional<double>> closing_EMA(const std::vector<OHLCV>& prices, int window);

std::vector<std::optional<BollingerBand>> BollBands(const std::vector<OHLCV>& prices, int window);

double stddev(const std::deque<double>& vals, double sma);

#endif