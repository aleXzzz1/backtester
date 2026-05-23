#include "Portfolio.h"
#include <cmath>

#define CAP_ALLOC 0.95

template <typename Event>
vector<Order> Portfolio<Event>::consider(const vector<Signal>& signals, const MarketContext<Event>& ctx) {
    // LIMITATION: Orders first in the signals vector are unintentionally assigned higher priority
    vector<Order> orders;
    if (signals.empty()) return orders; // return no orders if no signals were emitted by strategy
    for (const Signal& sig : signals) {
        const auto& latest = ctx.get_latest(sig.symbol); 
        // if (!latest) continue;
        double price = execution_price(latest);
        double current_qty = positions_[sig.symbol].quantity_;

        if (sig.direction == Direction::LONG) { 
            double target_qty = std::floor((total_equity(ctx) * CAP_ALLOC) / price); //Target 95% equity captital purchase
            double cost = target_qty * price;
            if (cost <= current_cash_ && target_qty > 0) {
                orders.push_back(Order{.volume_ = target_qty, .symbol = sig.symbol});
            }
        } else if (sig.direction == Direction::FLAT && current_qty > 0.0) {
            orders.push_back(Order{.volume_ = -1 * current_qty, .symbol = sig.symbol});
        }
    }
    return orders;
}

template <typename Event>
void Portfolio<Event>::apply(const vector<Fill>& fills, const MarketContext<Event>& context) {
    // For each fill, update our internal list of fills, update our position that fill
    // update current_cash_
    for (const Fill& f : fills) {
        fills_.push_back(f);
        update_position(f);
        current_cash_ -= f.total_cost_;
    }
    update_equitycurve(context);
}

template <typename Event>
void Portfolio<Event>::update_position(const Fill& f) {
    double prev_avg_cost = positions_[f.symbol].avgcost_;
    double prev_qty = positions_[f.symbol].quantity_;

    // Average cost of position for symbol over time
    double new_qty = prev_qty + f.volume_;
    double new_avg_cost = (new_qty == 0.0) ? 0.0 :
    (prev_avg_cost * prev_qty + f.price_ * f.volume_) / new_qty;

    positions_[f.symbol].avgcost_ = new_avg_cost;
    positions_[f.symbol].quantity_= new_qty;
}

template <typename Event>
void Portfolio<Event>::update_equitycurve(const MarketContext<Event>& cxt) {
    // For all open positions, determine most recent closing price of symbol and update equity curve for timepoint
    double esum = current_cash_;
    for (const auto& [symbol, position] : positions_) {
        const auto& latest = cxt.get_latest(symbol);
        esum += mark_price(latest) * position.quantity_;
    }
    equitycurve_.push_back(EquityPoint{.ts = cxt.get_time(), .equity = esum});
}

template <typename Event>
double Portfolio<Event>::total_equity(const MarketContext<Event>& cxt) {
    if (equitycurve_.empty()) return current_cash_;

    return equitycurve_.back().equity;
    
}