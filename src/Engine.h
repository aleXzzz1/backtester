#ifndef ENGINE_H
#define ENGINE_H

#include "MarketContext.h"
#include "DataFeed.h"
#include "EagerCSVFeed.h"
#include "MarketEvent.h"
#include "Broker.h"
#include "MAC.h"
#include "Portfolio.h"

template <typename Event>
class Engine {
    public:
    Engine(std::unique_ptr<DataFeed<Event>> feed,
           std::unique_ptr<Strategy<Event>> strategy)
        : feed_(std::move(feed)), strgy_(std::move(strategy)) {
            orderBuffer_.reserve(8);
        }

    void run();

    private:
    std::unique_ptr<DataFeed<Event>> feed_;
    MarketContext<Event> ctx_;
    std::unique_ptr<Strategy<Event>> strgy_;
    Portfolio portfolio_; // has no default constructor. must initialize in engine ctor
    Broker broker_;
    std::vector<Order> orderBuffer_;

};

#endif