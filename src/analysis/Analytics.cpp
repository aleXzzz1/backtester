#include "Analytics.h"
#include <cmath>
#include <chrono>
#include <iostream>
#include <numeric>

using std::vector;

PerformanceReport Analytics::compute(const vector<EquityPoint>& eqcurve, const vector<FillEvent>& fills,
                                     const MarketContext& ctx, const std::string& symbol) {
    EquityPoint init_eq = eqcurve.front();
    EquityPoint final_eq = eqcurve.back();
    auto time_diff = final_eq.ts - init_eq.ts;
    auto diff_years = std::chrono::duration_cast<std::chrono::years>(time_diff);
    daily_returns(eqcurve);
    // only for one symbol atm
    double first_open = mark_price(ctx.get_oldest(symbol));
    double last_open = mark_price(ctx.get_latest(symbol));


    double tr = total_return(init_eq.equity, final_eq.equity);
    double benchmark = benchmark_return(first_open, last_open);
    double cag_r = cagr(eqcurve);
    double m_drawdown = max_drawdown(eqcurve);
    int m_drawdown_days = max_drawdown_days(eqcurve);
    double shrp = sharpe();
    


    // double cagr = cagr(init_eq.equity, final_eq.equity, diff_years);
    return {.total_return = tr, .cagr = cag_r, .benchmark_return = benchmark, 
            .max_drawdown = m_drawdown,
            .sharpe = shrp, .max_drawdown_duration_days = m_drawdown_days};
    }

double Analytics::total_return(double init_eq, double final_eq) {
    return (final_eq / init_eq) - 1; // gives in decimal (eg. 0.25 = 25%)
}

double Analytics::benchmark_return(double first_open, double last_open) {
    return (last_open / first_open) - 1; 
}

double Analytics::max_drawdown(const vector<EquityPoint>& eqcurve) {
    double max_drawdown {0}; 
    double max {0};
    for (const EquityPoint& e : eqcurve) {
        if (e.equity > max) {
            max = e.equity;
        } else {
            double drawdown = (e.equity - max) / max;
            max_drawdown = (drawdown < max_drawdown) ? drawdown : max_drawdown;
        }
    }
    return max_drawdown;
}

int Analytics::max_drawdown_days(const vector<EquityPoint>& eqcurve) {
    timestamp max_drawdown_ts; 
    EquityPoint max_eq;
    double max_drawdown {0};
    for (const EquityPoint& e : eqcurve) {
        if (e.equity > max_eq.equity) {
            max_eq = e;
            max_drawdown_ts = e.ts;
        } else {
            double drawdown = (e.equity - max_eq.equity) / max_eq.equity;
            if (drawdown < max_drawdown) {
                max_drawdown = drawdown;
                max_drawdown_ts = e.ts;
            }
        }
    }
    auto diff = std::chrono::duration_cast<std::chrono::hours>(max_drawdown_ts - max_eq.ts);
    return (diff.count() / 24);
}


double Analytics::sharpe() {
    if (returns.size() < 2) return 0.0;
    double dr_sum = std::accumulate(returns.begin(), returns.end(), 0.0); 
    double dr_mean = dr_sum / returns.size();
    double variance {0};
    for (double r : returns) {
        variance += (r - dr_mean)*(r - dr_mean);
    }
    variance /= (returns.size() - 1);
    double stddev = std::sqrt(variance);
    return (dr_mean / stddev) * std::sqrt(252);
}

double Analytics::cagr(const vector<EquityPoint>& eqcurve) {
    EquityPoint final = eqcurve.back();
    EquityPoint starting = eqcurve.front();
    // std::cout << "Final equity is: $" << final.equity << std::endl;  // Diagnostic print lines
    // std::cout << "Starting equity is: $" << starting.equity << std::endl; // Diagnostic print lines
    auto diff_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(final.ts - starting.ts).count();
    auto diff_years = diff_seconds / (24.0*365.0*3600);
    // std::cout << "number of years difference is: " << diff_years << std::endl; // Diagnostic print lines
    double exp = (1 / diff_years);
    return std::pow((final.equity / starting.equity), exp) - 1;
}

/*
double Analytics::cagr(double init_eq, double final_eq, std::chrono::year years) {
    double base = (final_eq / init_eq);
    double exponent = (1 / years);
    return std::pow(base, exponent) - 1; // gives in decimal
}
*/

void Analytics::daily_returns(const vector<EquityPoint>& eqcurve) {
    for (int i = 1; i < eqcurve.size(); i++) {
        returns.push_back((eqcurve[i].equity / eqcurve[i - 1].equity) - 1.0);
    }
    // for (double r : returns) { std::cout << r << std::endl; }
}

void PerformanceReport::print_report() {
    std::cout << "Backtest run complete! Printing analytics...\n";
    std::cout << "============================================\n";

    std::cout << "Total Return:           " << total_return * 100 << "%" << std::endl; 
    std::cout << "CAGR:                   " << cagr * 100 << "%" << std::endl;
    std::cout << "Benchmark [Buy & Hold]: " << benchmark_return * 100 << "%" << std::endl;
    std::cout << "Max Drawdown:           " << max_drawdown * 100 << "%" << std::endl;
    std::cout << "Max Drawdown Days:      " << max_drawdown_duration_days << std::endl;
    std::cout << "Sharpe Ratio:           " << sharpe << "\n\n\n";
}
