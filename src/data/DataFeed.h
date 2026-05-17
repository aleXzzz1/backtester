#ifndef DATAFEED_H
#define DATAFEED_H

#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include "MarketEvent.h"

template <typename Event>
class DataFeed {

    public:
    virtual ~DataFeed() = default;
    virtual std::optional<Event> next();

    private:

};


#endif