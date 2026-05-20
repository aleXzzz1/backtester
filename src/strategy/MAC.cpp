#include "MAC.h"

using std::vector;
using std::optional;


vector<Signal> MACrossover::on_event(const OHLCV& bar, const MarketContext<OHLCV>& ctx) {
    vector<Signal> signals;
    optional<double> fast, slow;
    fast = fast_sma_cache_.update(bar.close_);
    slow = slow_sma_cache_.update(bar.close_);

    if (fast == std::nullopt || slow == std::nullopt) return signals; // empty signal list if insufficient SMA data

    bool above = (*fast > *slow);

    if (!initialized_) {        // Check if SMA state has been initialized
        initialized_ = true;
        was_above_ = above; 
        return signals;
    }

    if (above && !was_above_) signals.push_back({.symbol = this->symbol_, .direction = Direction::LONG});
    if (!above && was_above_) signals.push_back({.symbol = this->symbol_, .direction = Direction::FLAT});

    was_above_ = above;

    return signals;
}
