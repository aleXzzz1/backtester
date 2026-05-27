#include "MAC.h"
#include "Overloaded.h"

using std::vector;
using std::optional;


vector<SignalEvent> MACrossover::on_event(const MarketEvent& event, const MarketContext& ctx) {
    return std::visit(overloaded{
        [&](const Bar& bar) { return on_bar(bar); },
        [&](const Tick&) { return vector<SignalEvent>{}; }
    }, event);
}

vector<SignalEvent> MACrossover::on_bar(const Bar& bar) {
    vector<SignalEvent> signals;
    optional<double> fast, slow;
    fast = fast_sma_cache_.update(bar.close);
    slow = slow_sma_cache_.update(bar.close);

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
