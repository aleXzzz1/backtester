#include "Broker.h"
#include <iostream>

template <typename Event>
vector<Fill> Broker<Event>::fill(const vector<Order>& orders, const MarketContext<Event>& ctx) {
    vector<Fill> fills;
    if (orders.empty()) return fills; //return empty fill order if no orders came in
    for (const Order& o : orders) {
        const auto& latest = ctx.get_latest(o.symbol);
        double price = apply_slippage(o, execution_price(latest));
        double order_cost = (price * o.volume_);
        double total_cost = ((order_cost) * commission) + order_cost;
        fills.push_back(Fill{.price_ = price, 
                        .volume_ = o.volume_,
                        .total_cost_ = total_cost,
                        .commission_ = ((order_cost) * commission),
                        .submittedAt = ctx.get_time(),
                        .symbol = o.symbol});
        std::cout << "Executed trade for" << o.symbol << "at price/volume of " <<
         price << "/" << o.volume_ << std::endl;
    }
    return fills;
}

template <typename Event>
double Broker<Event>::apply_slippage(const Order& o, double exec_price) {
    double slip = exec_price * slippage;
    return exec_price + (o.volume_ > 0 ? slip : -slip);
}