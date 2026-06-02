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
        void runMRBBMenu();
        void runSMAStrategy(double short_ma, double long_ma, const std::string& symbol);
        PortfolioParam portfolioparam_ {10000.0, 0.95};
};

#endif