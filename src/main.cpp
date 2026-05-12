#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include "ohlcv.h"
#include "csv_parser.h"

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        // Print usage and exit
        std::cerr << "Usage: " << argv[0] << "<csv_file>" << std::endl;
        return 1;
    }

    std::ifstream file{argv[1]};

    if (!file) {
        // Print an error and exit
        std::cerr << "Uh oh, TSLA.csv could not be opened for reading!\n";
        return 1;
    }

    std::vector<OHLCV> vec = parseFile(file);

    for (const OHLCV& obj : vec) {
        std::cout << toString(obj.m_date) << toString(obj) << std::endl;
    }

    return 0;


}