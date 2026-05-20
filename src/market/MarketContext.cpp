#include "MarketContext.h" 

using std::deque;
using std::unordered_map;

template <typename Event>
void MarketContext<Event>::update(const Event& event, const std::string symbol) {
    if (history_[symbol].size() == maxHistorySize_) {
        history_[symbol].pop_front();
    }
    history_[symbol].push_back(event);
    currentTime_ = event_time(event);
}

template <typename Event>
const deque<Event>& MarketContext<Event>::history(const std::string& symbol) const {
    // TODO: Fix!!, it is not const since [] operator creates default key if not found
    return history_[symbol];
}

template <typename Event>
Event MarketContext<Event>::get_latest(const std::string& symbol) const {
    return = history_at(symbol).back();
}
