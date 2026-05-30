#ifndef MAC_H
#define MAC_H

#include "MarketContext.h"
#include "Event.h"
#include "Indicators.h"
#include "Strategy.h"
#include <string>
#include <vector>

class MACrossover : public Strategy {

    public:
        MACrossover(std::string symbol, int fast_period, int slow_period) : 
        Strategy(std::move(symbol)),
        fast_sma_cache_(fast_period),
        slow_sma_cache_(slow_period),
        fast_period_(fast_period),
        slow_period_(slow_period),
        was_above_ (false),
        initialized_(false)
        {}

        std::vector<SignalEvent> on_event(const MarketEvent& event, const MarketContext& ctx) override;

        std::string name() const override {return "MACrossover";}

    private:

        std::vector<SignalEvent> on_bar(const Bar& bar);
    SMA fast_sma_cache_;
    SMA slow_sma_cache_;
    int fast_period_, slow_period_;
    bool was_above_;
    bool initialized_; 

};

#endif