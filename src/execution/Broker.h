#ifndef BROKER_H
#define BROKER_H

#include "MarketContext.h"
#include "DataFeed.h"
#include "EagerCSVFeed.h"
#include "MarketEvent.h"
#include "Indicators.h"
#include <string>

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
    double total_cost_; // can be negative, representing a sell fill
    double commission_;
    timestamp submittedAt;
    std::string symbol;
};

template <typename Event>
class Broker {
    public:
        vector<Fill> fill(const vector<Order>& orders, const MarketContext<Event>& ctx);
    private:
        double apply_slippage(const Order& o, double exec_price);
    const double slippage {0.05};
    const double commission {0.02};

};

#include "Broker.tpp"

#endif