#ifndef MARKETCONTEXT_H
#define MARKETCONTEXT_H

#include <unordered_map>
#include <deque>
#include "MarketEvent.h"

// 

template <typename Event>
class MarketContext {

    public:
        void update(const Event& event, const std::string symbol);

        const std::deque<Event>& history(const std::string& symbol) const;

    private:

    std::unordered_map<std::string, std::deque<Event>> history_;
    std::chrono::system_clock::time_point currentTime_ {};
    std::size_t maxHistorySize_ {500};

};

#endif