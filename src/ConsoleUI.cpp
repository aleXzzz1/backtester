
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <limits>
#include "ConsoleUI.h"
#include "Engine.h"
#include "MAC.h"
#include "Analytics.h"
#include "LazyCSVFeed.h"
#include "Momentum.h"

void ConsoleUI::runORBMenu() {
    std::string fullLine;

    std::cout << "Input data to backtest on [Large SPY or smaller daily bars]\n";
    std::cout << "<Stock>\n";
    std::getline(std::cin >> std::ws, fullLine);
    std::stringstream ss(fullLine);

    std::string symbol;

    if (ss >> symbol) {
        runORBStrategy(symbol);
    }
}

void ConsoleUI::runORBStrategy(const std::string& symbol) {
    auto start = std::chrono::high_resolution_clock::now();
    std::string csvPath;
    if (symbol == "SPY") {
        csvPath = "data/spy_cleaned_v2.csv";
    }
    auto feed = std::make_unique<LazyCSVFeed>(csvPath);
    auto strategy = std::make_unique<MomentumORB>(symbol);
    Engine engine{std::move(feed), std::move(strategy), ORBPortfolioParam_};
    std::cout << "Running backtest on " << symbol << ": Opening Range Brakout (ORB), "
              << "$" << SMAPortfolioParam_.starting_cash << "\n";
    engine.run();
    auto curve = engine.get_port_curve();
    auto fills = engine.get_port_fills();
    const auto& context = engine.get_mkt_ctx();
    Analytics a;
    PerformanceReport report = a.compute(curve, fills, context, symbol);
    report.print_report();

    auto end = std::chrono::high_resolution_clock::now();

    auto diff = end - start;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

    std::cout << "Execution time: " << microseconds << " microseconds\n";
    std::cout << "Execution time: " << milliseconds << " milliseconds\n";

    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleUI::runSMASPY() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string symbol = "SPY";
    int short_ma = 2;
    int long_ma = 5;
    const std::string& csvPath = "data/spy_cleaned_v2.csv";
    auto feed = std::make_unique<LazyCSVFeed>(csvPath);
    auto strategy = std::make_unique<MACrossover>(symbol, short_ma, long_ma);
    Engine engine{std::move(feed), std::move(strategy), SMAPortfolioParam_};
    std::cout << "Running backtest on " << symbol << " (MAC " << short_ma << "/" << long_ma 
              << ", $" << SMAPortfolioParam_.starting_cash << "\n";
    engine.run();
    auto curve = engine.get_port_curve();
    auto fills = engine.get_port_fills();
    const auto& context = engine.get_mkt_ctx();
    Analytics a;
    PerformanceReport report = a.compute(curve, fills, context, symbol);
    report.print_report();

    auto end = std::chrono::high_resolution_clock::now();

    auto diff = end - start;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

    std::cout << "Execution time: " << microseconds << " microseconds\n";
    std::cout << "Execution time: " << milliseconds << " milliseconds\n";

    std::cout << "Press Enter to continue...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleUI::runSMAStrategy(double short_ma, double long_ma, const std::string& symbol) {
    auto start = std::chrono::high_resolution_clock::now();

    const std::string& csvPath = "data/" + symbol + ".csv";
    auto feed = std::make_unique<EagerCSVFeed>(csvPath);
    auto strategy = std::make_unique<MACrossover>(symbol, short_ma, long_ma);
    Engine engine{std::move(feed), std::move(strategy), SMAPortfolioParam_};
    std::cout << "Running backtest on " << symbol << " (MAC " << short_ma << "/" << long_ma 
              << ", $" << SMAPortfolioParam_.starting_cash << "\n";
    engine.run();
    auto curve = engine.get_port_curve();
    auto fills = engine.get_port_fills();
    const auto& context = engine.get_mkt_ctx();
    Analytics a;
    PerformanceReport report = a.compute(curve, fills, context, symbol);
    report.print_report();

    auto end = std::chrono::high_resolution_clock::now();

    auto diff = end - start;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

    std::cout << "Execution time: " << microseconds << " microseconds\n";
    std::cout << "Execution time: " << milliseconds << " milliseconds\n";

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
        std::cout << "[2] Simple Moving Average (SPY large data)\n";
        std::cout << "[3] Opening Range Breakout Momentum (SPY large data)\n";
        std::cout << "[4] Back\n";
        std::cin >> choice;

        switch (choice) {
            case 1:
                runSMAMenu();
                break;
            case 2:
                runSMASPY();
                break;
            case 3:
                runORBStrategy("SPY");
                break;
            case 4:
                break;
            default:
                std::cout << "Invalid Input. Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
        }
    } while (choice != 4);
}

void ConsoleUI::runPortfolioMenu() {
    std::string fullLine;
    std::cout << "Input Portfolio Parameters in the following format:\n";
    std::cout << "<starting_cash> <max_long_exposure>\n";
    std::getline(std::cin >> std::ws, fullLine);
    std::stringstream ss(fullLine);
    double starting_cash;
    double max_long_exposure;

    if (ss >> starting_cash >> max_long_exposure) {
        SMAPortfolioParam_.starting_cash = starting_cash;
        SMAPortfolioParam_.max_long_exposure = max_long_exposure;
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

// ORB strategy is wrong, analytics are wrong, FIX!!!