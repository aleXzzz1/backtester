#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "Broker.h"
#include "Portfolio.h"

struct PerformanceReport {
    // print perf report to console after backtest over
    void print_report();
    // returns
    double total_return;
    double cagr;
    // double benchmark_return;
    // double alpha;

    // risk
    // double max_drawdown;
    // int max_drawdown_duration_days;
    // double sharpe;
    // double sortino;
    // double calmar;

    /* trades
    int num_trades;
    double win_rate;
    double avg_win;
    double avg_loss;
    double profit_factor;
    double avg_trade_duration_days;
    */

    // costs
    //double total_commission;
    //double total_slippage;
    // double cost_as_pct_of_gross_pnl;
};

class Analytics {
    public:
    static PerformanceReport compute(
        const vector<EquityPoint>& eqcurve,
        const vector<Fill>& fills);

    private:
        static double total_return(double init_eq, double final_eq);
        static double cagr(double init_eq, double final_eq, std::chrono::year years);
};

#endif