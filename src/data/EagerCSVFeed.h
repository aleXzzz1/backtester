#ifndef EAGERCSVFEED_H
#define EAGERCSVFEED_H

#include <optional>
#include <vector>
#include <string>
#include <cstddef>
#include "DataFeed.h"

using std::optional;
using std::vector;
using std::string;

class EagerCSVFeed : public DataFeed {

    public:
        explicit EagerCSVFeed(const string& csvPath);

        optional<MarketEvent> next() override;

        std::size_t size() const { return events_.size(); }

        std::string get_symbol() const override { return symbol_; } 

    private:
        Bar parseLine(const string& line);
        std::chrono::system_clock::time_point parseDate(const string& s);
        vector<string> splitCSVLine(const string& line);
        void validateHeader(const string& line);

    vector<optional<Bar>> events_;
    std::string symbol_;
    size_t cursor_ {0};
    
};

#endif