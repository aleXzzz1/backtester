#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <unordered_map>
#include "Event.h"
#include "Broker.h"

using timestamp = std::chrono::system_clock::time_point;

struct EquityPoint {
    timestamp ts;
    double equity;
};

struct Position {
    double quantity_;
    double avgcost_;
    timestamp openedAt;
};

class Portfolio {
    public:
        Portfolio(double start_cash) : initial_cash_(start_cash)
        , current_cash_(start_cash) {}

        // Takes in a list of singals/desired orders from strategy
        // Outputs a list of actual order objects that can be considered by broker  
        // Takes into account current liquidty available as well as total equity
        OrderEvent consider(const SignalEvent& signals, const MarketContext& cxt);

        // Updates internal equity curve and position based on realized/filled orders
        void apply(const FillEvent& fill, const MarketContext& context);

        // Closes all open positions
        // void close();

        // Returns equity curve internal member variable
        std::vector<EquityPoint> get_equitycurve() {return equitycurve_;}

        std::vector<FillEvent> get_fills() { return fills_;}
        void update_equitycurve(const MarketContext& cxt);
        
    private:
        double total_equity(const MarketContext& cxt);
        void update_position(const FillEvent& fill);
    double initial_cash_;
    double current_cash_;
    std::unordered_map<std::string, Position> positions_; // Position indexed by symbol
    std::vector<EquityPoint> equitycurve_;
    std::vector<FillEvent> fills_;
};


#endif