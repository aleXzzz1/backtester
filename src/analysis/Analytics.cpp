#include "Analytics.h"
#include <cmath>
#include <chrono>
#include <iostream>

PerformanceReport Analytics::compute(const vector<EquityPoint>& eqcurve, const vector<FillEvent>& fills) {
    EquityPoint init_eq = eqcurve.front();
    EquityPoint final_eq = eqcurve.back();
    auto time_diff = final_eq.ts - init_eq.ts;
    auto diff_years = std::chrono::duration_cast<std::chrono::years>(time_diff);

    double tr = total_return(init_eq.equity, final_eq.equity);
    // double cagr = cagr(init_eq.equity, final_eq.equity, diff_years);
    return {.total_return = tr};
    }

double Analytics::total_return(double init_eq, double final_eq) {
    return (final_eq / init_eq) - 1; // gives in decimal (eg. 0.25 = 25%)
}

/*
double Analytics::cagr(double init_eq, double final_eq, std::chrono::year years) {
    double base = (final_eq / init_eq);
    double exponent = (1 / years);
    return std::pow(base, exponent) - 1; // gives in decimal
}
*/

void PerformanceReport::print_report() {
    std::cout << "Total return was: " << total_return * 100 << "%" << std::endl; 
}
