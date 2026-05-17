#ifndef BROKER_H
#define BROKER_H

#include "MarketContext.h"
#include "DataFeed.h"
#include "EagerCSVFeed.h"
#include "MarketEvent.h"
#include "Indicators.h"
#include <string>

enum class Side : uint8_t {
    BUY,
    SELL
};

struct Order {
    double price_, volume_;
    std::string symbol;
    Side side_;
    
};

#endif