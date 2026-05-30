#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include "DataFeed.h"
#include "EagerCSVFeed.h"
#include "Broker.h"
#include "MarketContext.h"
#include "Portfolio.h"
#include "MAC.h"
#include "Engine.h"
#include "Analytics.h"

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        // Print usage and exit
        std::cerr << "Usage: " << argv[0] << "<csv_file>" << std::endl;
        return 1;
    }
    auto feed = std::make_unique<EagerCSVFeed>(argv[1]);
    std::string symbol = feed->get_symbol();

    auto strategy = std::make_unique<MACrossover>(symbol, 20, 50);
    

    Engine engine{std::move(feed), std::move(strategy), 10000.0};

    // --- Run ---
    std::cout << "Running backtest on " << symbol
                << " (MAC " << 20 << "/" << 50
                << ", $" << 10000.0 << " starting)...\n";
    engine.run();

    auto curve = engine.get_port_curve();
    auto fills = engine.get_port_fills();
    const auto& context = engine.get_mkt_ctx();
    Analytics a;
    PerformanceReport report = a.compute(curve, fills, context, symbol);
    report.print_report();

    
    return 0;


}