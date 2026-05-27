#ifndef MARKETCONTEXT_H
#define MARKETCONTEXT_H

#include <unordered_map>
#include <deque>
#include "Event.h"

using timestamp = std::chrono::system_clock::time_point;

class MarketContext {

    public:
        void update(const MarketEvent& event);

        const std::deque<MarketEvent> history(const std::string& symbol) const;

        MarketEvent get_latest(const std::string& symbol) const;

        const timestamp get_time() const { return currentTime_; }

    private:

    std::unordered_map<std::string, std::deque<MarketEvent>> history_;
    timestamp currentTime_ {};
    std::size_t maxHistorySize_ {500};

};

#endif