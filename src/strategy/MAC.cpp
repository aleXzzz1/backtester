#include "MAC.h"

using std::vector;
using std::optional;

vector<Order> MACrossover::on_bar(const OHLCV& bar, const MarketContext<OHLCV>& ctx) {
    vector<Order> orders;
    optional<double> fast, slow;
    fast = fast_sma_cache_.update(bar.close_);
    slow = slow_sma_cache_.update(bar.close_);

    if (fast == std::nullopt || slow == std::nullopt) return orders; // empty order list if insufficient SMA data

    Order o;
    bool above = (*fast > *slow);
    if (above && !was_above_) orders.push_back({.symbol = symbol_, .volume_ = 100, .side_ = Side::BUY});
    if (!above && was_above_) orders.push_back({.symbol = symbol_, .volume_ = 100, .side_ = Side::SELL});
    was_above_ = above;

    return orders;
}