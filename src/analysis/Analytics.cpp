#include "Analytics.h"
#include <cmath>
#include <chrono>
#include <iostream>
#include <numeric>

using std::vector;

PerformanceReport Analytics::compute(const vector<EquityPoint>& eqcurve, const vector<FillEvent>& fills,
                                     const MarketContext& ctx, const std::string& symbol) {
    vector<double> returns = daily_returns(eqcurve);
    // only for one symbol atm
    double first_open = mark_price(ctx.get_oldest(symbol));
    double last_open = mark_price(ctx.get_latest(symbol));

    double start_e = eqcurve.front().equity;
    double final_e = eqcurve.back().equity;
    double tr = total_return(eqcurve);
    double benchmark = benchmark_return(first_open, last_open);
    double cag_r = cagr(eqcurve);
    double m_drawdown = max_drawdown(eqcurve);
    int m_drawdown_days = max_drawdown_days(eqcurve);
    double shrp = sharpe(returns);
    double t_commission = total_commission(fills);
    int number_trades = fills.size();
    int eqp = eqcurve.size();


    // double cagr = cagr(init_eq.equity, final_eq.equity, diff_years);
    return {.total_return = tr,
            .cagr = cag_r,
            .benchmark_return = benchmark, 
            .starting_equity = start_e,
            .final_equity = final_e,
            .max_drawdown = m_drawdown,
            .sharpe = shrp, .max_drawdown_duration_days = m_drawdown_days,
            .total_commission = t_commission,
            .num_trades = number_trades,
            .num_equity_points = eqp};
    }

double Analytics::total_return(const vector<EquityPoint>& eqcurve) {
    double init_eq = eqcurve.front().equity;
    double final_eq = eqcurve.back().equity;
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
    if (eqcurve.empty()) return 0;

    double max_drawdown_diff {0};
    EquityPoint max_eq {eqcurve.front()};
    bool underwater {false};

    for (const EquityPoint& e : eqcurve) {
        if (e.equity >= max_eq.equity) {
            if (underwater) {
                double diff = std::chrono::duration_cast<std::chrono::hours>(e.ts - max_eq.ts).count();
                max_drawdown_diff = (diff > max_drawdown_diff) ? diff : max_drawdown_diff;
                max_eq = e;
                underwater = false;
            }
        } else {
            underwater = true;
        }
    }
    
    // Compute diff for final equity point
    if (underwater) {
        double diff = std::chrono::duration_cast<std::chrono::hours>(eqcurve.back().ts - max_eq.ts).count();
        max_drawdown_diff = (diff > max_drawdown_diff) ? diff : max_drawdown_diff;
    }
    return (max_drawdown_diff / 24);
}


double Analytics::sharpe(const vector<double>& returns) {
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

vector<double> Analytics::daily_returns(const vector<EquityPoint>& eqcurve) {
    vector<double> returns;
    for (int i = 1; i < eqcurve.size(); i++) {
        returns.push_back((eqcurve[i].equity / eqcurve[i - 1].equity) - 1.0);
    }
    return returns;
}

double Analytics::total_commission(const std::vector<FillEvent>& fills) {
    double sum {0.0};
    for (const auto& f : fills) {
        sum += f.commission;
    }
    return sum;
}

void PerformanceReport::print_report() {
    std::cout << "Backtest run complete! Printing analytics...\n";
    std::cout << "============================================\n";


    std::cout << "Beginning Equity:       " << "$" << starting_equity << std::endl;
    std::cout << "Final Equity:           " << "$" << final_equity << std::endl;
    std::cout << "Total Return:           " << total_return * 100 << "%" << std::endl; 
    std::cout << "CAGR:                   " << cagr * 100 << "%" << std::endl;
    std::cout << "Benchmark [Buy & Hold]: " << benchmark_return * 100 << "%" << std::endl;
    std::cout << "Max Drawdown:           " << max_drawdown * 100 << "%" << std::endl;
    std::cout << "Max Drawdown Days:      " << max_drawdown_duration_days << std::endl;
    std::cout << "Sharpe Ratio:           " << sharpe << std::endl;
    std::cout << "Total Commission Cost:  " << "$" << total_commission  << std::endl;
    std::cout << "Number of Trades:       " << num_trades << std::endl;
    std::cout << "Number of Equity Points:" << num_equity_points << std::endl;
    std::cout << "\n";
}
