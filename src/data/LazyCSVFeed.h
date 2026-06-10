#ifndef LAZYCSVFEED_H
#define LAZYCSVFEED_H

#include <fstream>
#include "DataFeed.h"

class LazyCSVFeed : public DataFeed {

    public:
        explicit LazyCSVFeed(const std::string& csvPath);
        std::optional<MarketEvent> next() override;
        std::string get_symbol() const override { return symbol_; } 

    private:
    std::vector<std::string_view> split_s(std::string_view str, char del);
    timestamp parse_time(std::string_view sv);
    std::ifstream file_;
    std::string symbol_{"SPY"};
};

#endif