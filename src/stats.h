#ifndef STATS_H
#define STATS_H

#include <vector>
#include "ohlcv.h"
#include <iostream>
#include <fstream>
#include <optional>

void buildCSV(const std::vector<OHLCV>& vec);

std::vector<std::optional<double>> SMA(const std::vector<OHLCV>& vec);


void 

#endif