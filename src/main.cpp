#include "DataFeed.h"
#include "EagerCSVFeed.h"
#include "Broker.h"
#include "MarketContext.h"
#include "Portfolio.h"
#include "MAC.h"
#include "Engine.h"
#include "Analytics.h"
#include "ConsoleUI.h"


int main(int argc, char *argv[]) {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "Welcome to my C++ Backtester! Select an option to get started: \n\n";

    ConsoleUI cu;
    cu.runMainMenu();
    return 0;
}