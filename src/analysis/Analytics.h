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

    // Buy and Hold
    double benchmark_return;
    // double alpha;

    // risk
    double max_drawdown;
    int max_drawdown_duration_days;
    double sharpe;
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
    PerformanceReport compute(
        const std::vector<EquityPoint>& eqcurve,
        const std::vector<FillEvent>& fills,
        const MarketContext& ctx,
        const std::string& symbol);

    private:
        double total_return(double init_eq, double final_eq);
        double cagr(const std::vector<EquityPoint>& eqcurve);
        double total_comission();
        double total_slippage();
        double benchmark_return(double first_open, double last_open);
        double max_drawdown(const std::vector<EquityPoint>& eqcurve);
        int max_drawdown_days(const std::vector<EquityPoint>& eqcurve);
        double sharpe();
        void daily_returns(const std::vector<EquityPoint>& eqcurve);
        double cagr(double init_eq, double final_eq, std::chrono::year years);
        std::vector<double> returns;
};
#endif