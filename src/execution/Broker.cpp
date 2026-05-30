#include "Broker.h"
#include <iostream>

FillEvent Broker::fill(const OrderEvent& order, const MarketContext& ctx) {
    const auto& latest = ctx.get_latest(order.symbol);
    double price = apply_slippage(order, execution_price(latest));
    double order_cost = (price * order.volume);
    double total_cost = ((order_cost) * commission) + order_cost;
    // std::cout << "Executed trade for \"" << order.symbol << "\" at price/volume of " <<
    //     price << "/" << order.volume << std::endl;
    return FillEvent{.price = price, 
                     .volume = order.volume,
                     .total_cost = total_cost,
                     .commission = ((order_cost) * commission),
                     .ts = ctx.get_time(),
                     .symbol = order.symbol};
}

double Broker::apply_slippage(const OrderEvent& o, double exec_price) {
    double slip = exec_price * slippage;
    return exec_price + (o.volume > 0 ? slip : -slip);
}