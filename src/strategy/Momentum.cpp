#include "Momentum.h"
#include "Overloaded.h"
#include <iostream>
#include <ctime>
#include <iomanip>

using std::vector;

vector<SignalEvent> MomentumORB::on_event(const MarketEvent& event, const MarketContext& cxt) {
    return std::visit(overloaded{
        [&](const Bar& bar) { return on_bar(bar); },
        [&](const Tick&) { return vector<SignalEvent>{}; }
    }, event);
}

vector<SignalEvent> MomentumORB::on_bar(const Bar& bar) {
    vector<SignalEvent> signals;
    auto this_day = std::chrono::floor<std::chrono::days>(bar.ts);
    // Check if new trading day
    if (this_day != currentday_) {
        resetOBRState(this_day, bar);
        return signals;
    }
    if (tradingDayEnded) return signals;
    // Check if we are still in formation window
    if (inFormationWindow_) {
        monitorHighLow(bar);
        return signals;
    }
    
    bool longStopLossExit = (lastEmit == Direction::LONG && bar.low <= currentOpenLow_);
    bool shortStopLossExit = (lastEmit == Direction::SHORT && bar.high >= currentOpenHigh_);

    // Current time
    std::time_t time_now = std::chrono::system_clock::to_time_t(bar.ts);
    std::tm* local_tm = std::localtime(&time_now);

    // Liquidate all positions if so we are ending trading day or long/short stop loss exit fire. 
    if (isTradingDayEnding(bar) || longStopLossExit || shortStopLossExit) {
        signals.emplace_back(SignalEvent{.symbol = bar.symbol, .ts = bar.ts, .direction = Direction::FLAT});
        std::cout << "Emitted a FLAT  signal at " << std::put_time(local_tm, "%d-%m-%Y %H:%M:%S") << std::endl;
        lastEmit = Direction::FLAT;
        return signals;
    }

    // One trade per day
    if (tradeOccurred_) return signals;

    if (bar.close > currentOpenHigh_) {
        signals.emplace_back(SignalEvent{.symbol = bar.symbol, .ts = bar.ts, .direction = Direction::LONG, .stop = currentOpenLow_});
        std::cout << "Emitted a LONG  signal at " << std::put_time(local_tm, "%d-%m-%Y %H:%M:%S") << std::endl;
        lastEmit = Direction::LONG;
        tradeOccurred_ = true;
    } else if (bar.close < currentOpenLow_) {
        signals.emplace_back(SignalEvent{.symbol = bar.symbol, .ts = bar.ts, .direction = Direction::SHORT, .stop = currentOpenHigh_});
        std::cout << "Emitted a SHORT signal at " << std::put_time(local_tm, "%d-%m-%Y %H:%M:%S") << std::endl;
        lastEmit = Direction::SHORT;
        tradeOccurred_ = true;
    }
    return signals;

}

void MomentumORB::monitorHighLow(const Bar& bar) {
    auto time_diff = std::chrono::duration_cast<std::chrono::minutes>(bar.ts - cdaystartingtime_).count();
    if (bar.high > currentOpenHigh_) {
        currentOpenHigh_ = bar.high;
    }
    if (bar.low < currentOpenLow_) {
        currentOpenLow_ = bar.low; 
    }
    if (time_diff >= openingFormationLength_) inFormationWindow_ = false;
}

void MomentumORB::resetOBRState(const std::chrono::sys_days& this_day, const Bar& bar) {
    tradingDayEnded = false;
    tradeOccurred_ = false;
    currentday_ = this_day;
    currentOpenHigh_ = bar.high;
    currentOpenLow_ = bar.low; // Initiate currentOpenHigh/Low to first bar.
    cdaystartingtime_ = bar.ts; // Set starting time of current trading day to first opening bar received.
    inFormationWindow_ = true;
}

// Check if we are 15-minutes away from market close.
bool MomentumORB::isTradingDayEnding(const Bar& bar) {
    // returns true if the difference in minutes between current bar and starting bar time is greater than
    // 375 minutes or 6.25 hours (15 minutes before trading hours close).
    bool result = std::chrono::duration_cast<std::chrono::minutes>(bar.ts - cdaystartingtime_).count() >= 375;
    if (result) tradingDayEnded = true;
    return result;
}