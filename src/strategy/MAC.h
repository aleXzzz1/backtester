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

    virtual std::vector<Order> on_bar(const Event& event, const MarketContext<Event>& cxt) = 0;

    virtual std::string name() const;

protected:
    std::string symbol_;

};

class MACrossover : public Strategy<OHLCV> {

    public:
        MACrossover(std::string symbol, int fast_period, int slow_period) : 
        fast_sma_cache_(fast_period),
        slow_sma_cache_(slow_period),
        fast_period_(fast_period),
        slow_period_(slow_period)
        { this->symbol_ = symbol; }

        std::vector<Order> on_bar(const OHLCV& bar, const MarketContext<OHLCV>& ctx) override;

        std::string name() const override;

    private:
    SMA fast_sma_cache_;
    SMA slow_sma_cache_;
    int fast_period_, slow_period_;
    bool was_above_; 

};

#endif