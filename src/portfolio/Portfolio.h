#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <unordered_map>
#include "MarketEvent.h"
#include "Broker.h"

using std::vector;
using timestamp = std::chrono::system_clock::time_point;

struct EquityPoint {
    timestamp timestamp;
    double equity;
};

struct Position {
    double quantity_;
    double avgcost_;
    timestamp openedAt;
};

template <typename Event>
class Portfolio {
    public:
        Portfolio(double start_cash) : initial_cash_(start_cash)
        , current_cash_(start_cash) {}

        // Takes in a list of singals/desired orders from strategy
        // Outputs a list of actual order objects that can be considered by broker  
        // Takes into account current liquidty available as well as total equity
        vector<Order> consider(const vector<Signal>& signals, const MarketContext<Event>& cxt);

        // Updates internal equity curve and position based on realized/filled orders
        void apply(const vector<Fill>& fills, const MarketContext<Event>& context);

        // Closes all open positions
        void close();

        // Returns equity curve internal member variable
        vector<EquityPoint> get_equitycurve();
    private:
        double total_equity(const MarketContext<Event>& cxt);
        void update_position(const Fill& fill);
        void update_equitycurve(const MarketContext<Event>& cxt);
    double initial_cash_;
    double current_cash_;
    std::unordered_map<std::string, Position> positions_; // Position indexed by symbol
    vector<EquityPoint> equitycurve_;
    vector<Fill> fills_;
};

#endif