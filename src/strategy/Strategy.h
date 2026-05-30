#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include <string>
#include "Event.h"
#include "MarketContext.h"

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual std::vector<SignalEvent> on_event(const MarketEvent& event, const MarketContext& cxt) = 0;

    virtual std::string name() const = 0;

protected:
    explicit Strategy(std::string symbol) : symbol_(std::move(symbol)) {}
    std::string symbol_;
};

#endif