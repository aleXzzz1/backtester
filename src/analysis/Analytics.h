#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "Broker.h"
#include "Portfolio.h"

struct PerformanceReport {
    // print perf report to console after backtest over
    void print_report();

    // returns
    double starting_equity;
    double final_equity;
    double total_return;
    double cagr;

    // Buy and Hold
    double benchmark_return;
    // double alpha;

    // risk
    double max_drawdown;
    int max_drawdown_duration_days;
    double sharpe;
    // double sortino;
    // double calmar;

    // trades
    int num_trades;
    // double win_rate;
    // double avg_win;
    // double avg_loss;
    // double profit_factor;
    // double avg_trade_duration_days;
    

    // costs
    double total_commission;
    // double cost_as_pct_of_gross_pnl;
};

class Analytics {
    public:
    static PerformanceReport compute(
        const std::vector<EquityPoint>& eqcurve,
        const std::vector<FillEvent>& fills,
        const MarketContext& ctx,
        const std::string& symbol);

        static double total_return(const std::vector<EquityPoint>& eqcurve);
        static double cagr(const std::vector<EquityPoint>& eqcurve);
        static double total_commission(const std::vector<FillEvent>& fills);
        static double benchmark_return(double first_open, double last_open);
        static double max_drawdown(const std::vector<EquityPoint>& eqcurve);
        static int max_drawdown_days(const std::vector<EquityPoint>& eqcurve);
        static double sharpe(const std::vector<double>& returns);
        static std::vector<double> daily_returns(const std::vector<EquityPoint>& eqcurve);
        static double cagr(double init_eq, double final_eq, std::chrono::year years);
};
#endif