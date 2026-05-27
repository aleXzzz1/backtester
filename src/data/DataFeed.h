#ifndef DATAFEED_H
#define DATAFEED_H

#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include "Event.h"

class DataFeed {

    public:
    virtual ~DataFeed() = default;
    virtual std::optional<MarketEvent> next() = 0;
    virtual std::string get_symbol() const = 0;

    private:

};

#endif