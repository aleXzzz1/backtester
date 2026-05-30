#ifndef EAGERCSVFEED_H
#define EAGERCSVFEED_H

#include <optional>
#include <vector>
#include <string>
#include <cstddef>
#include "DataFeed.h"

class EagerCSVFeed : public DataFeed {

    public:
        explicit EagerCSVFeed(const std::string& csvPath);

        std::optional<MarketEvent> next() override;

        std::size_t size() const { return events_.size(); }

        std::string get_symbol() const override { return symbol_; } 

    private:
        Bar parseLine(const std::string& line);
        std::chrono::system_clock::time_point parseDate(const std::string& s);
        std::vector<std::string> splitCSVLine(const std::string& line);
        void validateHeader(const std::string& line);

    std::vector<std::optional<Bar>> events_;
    std::string symbol_;
    std::size_t cursor_ {0};
    
};

#endif