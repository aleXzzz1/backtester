#include "csv_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

Date parseDate(const std::string& s) {
    Date date;
    std::stringstream ss(s);
    std::string t;
    char del = '-';
    // Year
    getline(ss, t, del);
    date.year = std::stoi(t);

    // Month
    getline(ss, t, del);
    date.month = std::stoi(t);

    // Day
    getline(ss, t);
    date.day = std::stoi(t);
    
    return date;
}

OHLCV parseRow(const std::string& line) {
    std::stringstream ss(line);
    std::string t; // temporary string
    char del = ',';

    // date
    getline(ss, t, del); // t = "2019-02-04" for example
    Date date = parseDate(t);

    // close
    getline(ss, t, del); // t = "20.674667358398438" for example
    double close = std::stod(t);
    getline(ss,t,del); // because we have two close columns for some reason

    // high
    getline(ss, t, del);
    double high = std::stod(t);

    // low
    getline(ss, t, del);
    double low = std::stod(t);

    // open
    getline(ss, t, del);
    double open = std::stod(t);

    // volume
    getline(ss, t);
    std::int64_t volume = std::stoll(t);

    return OHLCV{open, high, low, close, volume, date};
}

std::vector<OHLCV> parseFile(std::ifstream& file) {
    // TODO: parse header to determine column order
    std::string line;
    std::vector<OHLCV> vec;
    getline(file, line);
    getline(file, line);
    getline(file, line);
    std::cout << line << "\n";
    while (getline(file, line)) {
        try {
        vec.push_back(parseRow(line));
        } catch (const std::exception& e) {
            std::cerr << "Parse error on line: [" << line << "] — " << e.what() << "\n";
        }
    }
    return vec;

}