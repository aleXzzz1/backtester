#include "MarketContext.h"

using std::deque;
using std::unordered_map;

void MarketContext::update(const MarketEvent& event) {
    const std::string symbol = event_symbol(event);
    if (history_[symbol].size() == maxHistorySize_) {
        history_[symbol].pop_front();
    }
    history_[symbol].push_back(event);
    currentTime_ = event_time(event);
}

const deque<MarketEvent> MarketContext::history(const std::string& symbol) const {
    return history_.at(symbol);
}

MarketEvent MarketContext::get_latest(const std::string& symbol) const {
    return history_.at(symbol).back();
}
