#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <fstream>
#include <string>
#include <vector>
#include "MarketEvent.h"

Date parseDate(const std::string& s);
OHLCV parseRow(const std::string& line);
std::vector<OHLCV> parseFile(std::ifstream& file);

#endif