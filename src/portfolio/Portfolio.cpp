#include "Portfolio.h"
#include <cmath>
#include <iostream>

#define CAP_ALLOC 0.95

OrderEvent Portfolio::consider(const SignalEvent& signal, const MarketContext& ctx) {
    // LIMITATION: Orders first in the signals vector are unintentionally assigned higher priority
    const auto& latest = ctx.get_latest(signal.symbol); // could be Market or Tick
    // if (!latest) continue;
    double price = execution_price(latest);
    double current_qty = positions_[signal.symbol].quantity_;

    if (signal.direction == Direction::LONG) { 
        double target_qty = std::floor((total_equity(ctx) * CAP_ALLOC) / price); //Target 95% equity captital purchase
        double cost = target_qty * price;
        if (cost <= current_cash_ && target_qty > 0) {
            return OrderEvent{.volume = target_qty, .symbol = signal.symbol};
        }
    } else if (signal.direction == Direction::FLAT && current_qty > 0.0) {
        return OrderEvent{.volume = -1 * current_qty, .symbol = signal.symbol};
    }
}

void Portfolio::apply(const FillEvent& fill, const MarketContext& context) {
    fills_.push_back(fill);
    update_position(fill);
    current_cash_ -= fill.total_cost;
}

void Portfolio::update_position(const FillEvent& f) {
    double prev_avg_cost = positions_[f.symbol].avgcost_;
    double prev_qty = positions_[f.symbol].quantity_;

    // Average cost of position for symbol over time
    double new_qty = prev_qty + f.volume;
    double new_avg_cost = (new_qty == 0.0) ? 0.0 :
    (prev_avg_cost * prev_qty + f.price * f.volume) / new_qty;

    positions_[f.symbol].avgcost_ = new_avg_cost;
    positions_[f.symbol].quantity_= new_qty;
}

void Portfolio::update_equitycurve(const MarketContext& cxt) {
    // For all open positions, determine most recent closing price of symbol and update equity curve for timepoint
    double esum = current_cash_;
    for (const auto& [symbol, position] : positions_) {
        const auto& latest = cxt.get_latest(symbol);
        esum += mark_price(latest) * position.quantity_;
    }
   // std::cout << "Equity at " << cxt.get_time() << " is: $" << esum << std::endl;
    equitycurve_.emplace_back(EquityPoint{.ts = cxt.get_time(), .equity = esum});
}

double Portfolio::total_equity(const MarketContext& cxt) {
    if (equitycurve_.empty()) return current_cash_;
    return equitycurve_.back().equity;
    
}