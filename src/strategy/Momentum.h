#ifndef MOMENTUM_H
#define MOMENTUM_H
#include "Strategy.h"

class MomentumORB : public Strategy {

    public:
    MomentumORB(const std::string& symbol) : Strategy(std::move(symbol)) {}
    std::vector<SignalEvent> on_event(const MarketEvent& event, const MarketContext& cxt) override;

    std::string name() const override {return "Momentum-Opening Range Breakout";}

    private:
    std::vector<SignalEvent> on_bar(const Bar& bar);
    void resetOBRState(const std::chrono::sys_days& this_day, const Bar& bar);
    void monitorHighLow(const Bar& bar);
    bool isTradingDayEnding(const Bar& bar);
    std::chrono::sys_days currentday_;
    std::chrono::system_clock::time_point cdaystartingtime_;
    Direction lastEmit {Direction::FLAT};
    bool tradeOccurred_ {false};
    bool tradingDayEnded {false};
    bool inFormationWindow_ {false};
    double currentOpenHigh_ {0.0};
    double currentOpenLow_ {0.0};
    int openingFormationLength_ {30};
};

#endif