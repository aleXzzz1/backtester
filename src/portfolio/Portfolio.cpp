#include "Portfolio.h"
#include <cmath>
#include <iostream>

std::optional<OrderEvent> Portfolio::consider(const SignalEvent& signal, const MarketContext& ctx) {
    // LIMITATION: Orders first in the signals vector are unintentionally assigned higher priority
    const auto& latest = ctx.get_latest(signal.symbol); // could be Market or Tick
    // if (!latest) continue;
    double price = execution_price(latest);
    double current_qty = positions_[signal.symbol].quantity_;

    if (signal.direction == Direction::FLAT) {
        if (current_qty != 0.0) return OrderEvent{.volume = -current_qty, .symbol = signal.symbol, .ts = signal.ts};
        return std::nullopt;
    }

    double stop_dist = std::abs(price - signal.stop);
    if (stop_dist == 0.0) {
        std::cout << "stop dist was equal to price!\n";
        return std::nullopt;
    }
    double risk_qty = std::floor((total_equity(ctx) * risk_per_trade) / price);
    double cap_qty  = std::floor((total_equity(ctx) * cap_alloc) / price);
    double target_qty = std::min(risk_qty, cap_qty);
    if (target_qty <= 0.0) return std::nullopt;

    if (signal.direction == Direction::LONG) { 
        //Target 95% equity captital purchase
        double cost = target_qty * price; 
        if (cost <= current_cash_) {
            //std::cout << "returning buy order\n";
            return OrderEvent{.volume = target_qty, .symbol = signal.symbol, .ts = signal.ts};
        }
    } else if (signal.direction == Direction::SHORT) {
        // SIMPLIFYING ASSUMPTION: Shorting has no margin constrain
        if (target_qty > 0.0) {
            return OrderEvent{.volume = -target_qty - current_qty, .symbol = signal.symbol, .ts = signal.ts};
        }
    }
    return std::nullopt;
    //std::cout << "Nothing happend";
}

void Portfolio::apply(const FillEvent& fill, const MarketContext& context) {
    fills_.push_back(fill);
    update_position(fill);
    current_cash_ -= fill.total_cost;
}

void Portfolio::update_position(const FillEvent& f) {
    Position& pos = positions_.at(f.symbol);
    double prev_avg_cost = pos.avgcost_;
    double prev_qty = pos.quantity_;
    double new_qty = prev_qty + f.volume;

    if (prev_qty == 0.0) {
        // Average cost is just fill price if no previous qty
        pos.avgcost_ = f.price;
    } else if ((prev_qty > 0.0) == (f.volume > 0)) {
        // If prev and cur qty are same sign, take weighted average
        pos.avgcost_ = (prev_avg_cost * prev_qty + f.price * f.volume) / new_qty;
    } else {
        // If prev and cur qty are diff sign
        if (new_qty == 0.0) {
            // Average cost is just fill price 
            pos.avgcost_ = 0.0;
        } else if ((prev_qty > 0.0) != (new_qty > 0.0)) {
            pos.avgcost_ = f.price;
        }
    }
    pos.quantity_ = new_qty;
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