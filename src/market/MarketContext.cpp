#include "MarketContext.h" 

using std::deque;
using std::unordered_map;

template <typename Event>
void MarketContext<Event>::update(const Event& event, const std::string symbol) {
    if (history_[symbol].size() == maxHistorySize_) {
        history_[symbol].pop_front();
    }
    history_[symbol].push_back(event);
}

template <typename Event>
const deque<Event>& MarketContext<Event>::history(const std::string& symbol) {
    return history_[symbol];
}
