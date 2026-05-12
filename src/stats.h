#ifndef STATS_H
#define STATS_H

#include <vector>
#include "ohlcv.h"
#include <iostream>
#include <fstream>
#include <optional>

void buildCSV(const std::vector<OHLCV>& vec);

std::vector<std::optional<double>> closing_SMA(const std::vector<OHLCV>& vec, int n);
std::vector<std::optional<double>> closing_EMA(const std::vector<OHLCV>& vec, int n);

#endif