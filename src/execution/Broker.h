#ifndef BROKER_H
#define BROKER_H

#include "MarketContext.h"
#include "DataFeed.h"
#include "EagerCSVFeed.h"
#include "Event.h"
#include "Indicators.h"
#include <string>

class Broker {
    public:
        FillEvent fill(const OrderEvent& order, const MarketContext& ctx);
    private:
        double apply_slippage(const OrderEvent& o, double exec_price);
    const double slippage {0.05};
    const double commission {0.02};

};


#endif