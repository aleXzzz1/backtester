#ifndef ENGINE_H
#define ENGINE_H

#include "MarketContext.h"
#include "DataFeed.h"
#include "Broker.h"
#include "MAC.h"
#include "Portfolio.h"
#include "Event.h"
#include <queue>

class Engine {
    public:
    Engine(std::unique_ptr<DataFeed> feed,
           std::unique_ptr<Strategy> strategy,
            double start_cash)
        : feed_(std::move(feed)), strgy_(std::move(strategy)),
        portfolio_(start_cash) {
            
        }

    void run();

    vector<EquityPoint> get_port_curve() {
        return portfolio_.get_equitycurve();
    }

    vector<FillEvent> get_port_fills() {
        return portfolio_.get_fills();
    }

    private:
    void handle(const MarketEvent& event);
    void handle(const SignalEvent& signal);
    void handle(const OrderEvent& order);
    void handle(const FillEvent& fill);

    std::queue<Event> eventQ;
    MarketContext ctx_;
    Portfolio portfolio_; // has no default constructor. must initialize in engine ctor
    Broker broker_;
    std::unordered_map<std::string, std::vector<OrderEvent>> pending_orders;
    std::unique_ptr<DataFeed> feed_;
    std::unique_ptr<Strategy> strgy_;



};

#endif