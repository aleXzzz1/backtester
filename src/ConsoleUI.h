#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "Portfolio.h"

class ConsoleUI {
    public: 
        void runMainMenu();
    private:
        void runPortfolioMenu();
        void runTradingStrategyMenu();
        void runSMAMenu();
        void runORBMenu();
        void runSMASPY();

        void runSMAStrategy(double short_ma, double long_ma, const std::string& symbol);
        void runORBStrategy(const std::string& symbol);
        PortfolioParam SMAPortfolioParam_ {10000.0, 0.95, 0.0, 0.0};
        PortfolioParam ORBPortfolioParam_ {10000.0, 0.95, 0.95, 0.10};
};

#endif