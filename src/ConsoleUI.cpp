
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <limits>
#include "ConsoleUI.h"
#include "Engine.h"
#include "MAC.h"
#include "Analytics.h"


void ConsoleUI::runMRBBMenu() {

}

void ConsoleUI::runSMAStrategy(double short_ma, double long_ma, const std::string& symbol) {
    const std::string& csvPath = "data/" + symbol + ".csv";
    auto feed = std::make_unique<EagerCSVFeed>(csvPath);
    auto strategy = std::make_unique<MACrossover>(symbol, short_ma, long_ma);
    Engine engine{std::move(feed), std::move(strategy), portfolioparam_};
    std::cout << "Running backtest on " << symbol << " (MAC " << short_ma << "/" << long_ma 
              << ", $" << portfolioparam_.starting_cash << "\n";
    engine.run();
    auto curve = engine.get_port_curve();
    auto fills = engine.get_port_fills();
    const auto& context = engine.get_mkt_ctx();
    Analytics a;
    PerformanceReport report = a.compute(curve, fills, context, symbol);
    report.print_report();

    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleUI::runSMAMenu() {
    std::string fullLine;

    std::cout << "Input SMA strategy parameters and stock in the following format:\n";
    std::cout << "<Short MA> <Long MA> <Stock>\n";
    std::getline(std::cin >> std::ws, fullLine);
    std::stringstream ss(fullLine);
    
    double short_ma;
    double long_ma;
    std::string symbol;

    if (ss >> short_ma >> long_ma >> symbol) {
        runSMAStrategy(short_ma, long_ma, symbol);
    }
}

void ConsoleUI::runTradingStrategyMenu() {
    int choice;
    do {
        std::cout << "========== STRATEGIES ==========\n";
        std::cout << "[1] Simple Moving Average Crossover\n";
        std::cout << "[2] Mean Reversion (Bollinger Bands) [CURRENTLY NOT FUNCTIONAL]\n";
        std::cout << "[3] Back\n";
        std::cin >> choice;

        switch (choice) {
            case 1:
                runSMAMenu();
                break;
            case 2:
                runMRBBMenu();
                break;
            case 3:
                break;
            default:
                std::cout << "Invalid Input. Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
        }
    } while (choice != 3);
}

void ConsoleUI::runPortfolioMenu() {
    std::string fullLine;
    std::cout << "Input Portfolio Parameters in the following format:\n";
    std::cout << "<starting_cash> <capital_alloc>\n";
    std::getline(std::cin >> std::ws, fullLine);
    std::stringstream ss(fullLine);
    double starting_cash;
    double capital_alloc;

    if (ss >> starting_cash >> capital_alloc) {
        portfolioparam_.starting_cash = starting_cash;
        portfolioparam_.capital_alloc = capital_alloc;
        std::cout << "Portfolio parameters successfully modified!\n";
    }

}

void ConsoleUI::runMainMenu() {
    int choice;
    do {
        std::cout << "========= OPTIONS ==========\n";
        std::cout << "[1] Pick a trading strategy\n";
        std::cout << "[2] Modify portfolio parameters\n";
        std::cout << "[3] Exit\n";
        std::cin >> choice;

        switch (choice) {
            case 1:
                runTradingStrategyMenu();
                break;
            case 2:
                runPortfolioMenu();
                break;
            case 3:
                std::cout << "Exiting backtester...\n";
                break;
            default:
                std::cout << "Invalid input. Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
        }
    } while (choice != 3);
}