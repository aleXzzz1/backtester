#ifndef MARKETCONTEXT_H
#define MARKETCONTEXT_H

#include <unordered_map>
#include <deque>
#include "MarketEvent.h"

using timestamp = std::chrono::system_clock::time_point;

template <typename Event>
class MarketContext {

    public:
        void update(const Event& event, const std::string symbol);

        const std::deque<Event>& history(const std::string& symbol) const;

        Event get_latest(const std::string& symbol) const;

        const timestamp get_time() const { return currentTime_; }

    private:

    std::unordered_map<std::string, std::deque<Event>> history_;
    timestamp currentTime_ {};
    std::size_t maxHistorySize_ {500};

};

#include "MarketContext.tpp"

#endif