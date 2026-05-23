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
           std::unique_ptr<Strategy<Event>> strategy,
            double start_cash)
        : feed_(std::move(feed)), strgy_(std::move(strategy)),
        portfolio_(start_cash) {
            orderBuffer_.reserve(8);
        }

    void run();

    vector<EquityPoint> get_port_curve() {
        return portfolio_.get_equitycurve();
    }

    vector<Fill> get_port_fills() {
        return portfolio_.get_fills();
    }

    private:
    std::unique_ptr<DataFeed<Event>> feed_;
    MarketContext<Event> ctx_;
    std::unique_ptr<Strategy<Event>> strgy_;
    Portfolio<OHLCV> portfolio_; // has no default constructor. must initialize in engine ctor
    Broker<OHLCV> broker_;
    std::vector<Order> orderBuffer_;

};

#include "Engine.tpp"

#endif