#ifndef BROKER_H
#define BROKER_H

#include "MarketContext.h"
#include "DataFeed.h"
#include "EagerCSVFeed.h"
#include "MarketEvent.h"
#include "Indicators.h"
#include <string>

#define COMISSION 2
#define SLIPPAGE 0.05

enum class Direction { LONG, FLAT, SHORT };
using timestamp = std::chrono::system_clock::time_point;

struct Signal {
    std::string symbol;
    Direction direction;
};

struct Order {
    double volume_;
    std::string symbol;
    
};

struct Fill {
    double price_;
    double volume_;
    double total_cost_;
    timestamp submittedAt;
    std::string symbol;
};

class Broker {
    public:
        vector<Fill> fill(const vector<Order>& orders, double open_price);
    private:

};

#endif