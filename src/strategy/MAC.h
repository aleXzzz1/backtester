#ifndef MAC_H
#define MAC_H

#include "MarketContext.h"
#include "DataFeed.h"
#include "EagerCSVFeed.h"
#include "MarketEvent.h"
#include "Indicators.h"
#include "Broker.h"
#include <string>

template <typename Event>
class Strategy {
public:
    virtual ~Strategy() = default;

    virtual std::vector<Signal> on_event(const Event& event, const MarketContext<Event>& cxt) = 0;

    virtual std::string name() const = 0;

protected:
    explicit Strategy(std::string symbol) : symbol_(std::move(symbol)) {}
    std::string symbol_;

};

class MACrossover : public Strategy<OHLCV> {

    public:
    using Strategy<OHLCV>::symbol_;
        MACrossover(std::string symbol, int fast_period, int slow_period) : 
        Strategy<OHLCV>(std::move(symbol)),
        fast_sma_cache_(fast_period),
        slow_sma_cache_(slow_period),
        fast_period_(fast_period),
        slow_period_(slow_period),
        was_above_ (false),
        initialized_(false)
        {}

        std::vector<Signal> on_event(const OHLCV& bar, const MarketContext<OHLCV>& ctx) override;

        std::string name() const override;

    private:
    SMA fast_sma_cache_;
    SMA slow_sma_cache_;
    int fast_period_, slow_period_;
    bool was_above_;
    bool initialized_; 

};

#endif