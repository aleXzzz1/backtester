#include <optional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

#include "MarketEvent.h"
#include "EagerCSVFeed.h"

EagerCSVFeed::EagerCSVFeed(const std::string& csvPath) {
    std::ifstream file(csvPath);
    if (!file) throw std::runtime_error("Could not open CSV: " + csvPath);
    string line;

    // Check that header is not missing columns
    getline(file, line);
    validateHeader(line);

    // Extract ticker symbol of csv file
    char del = ',';
    getline(file, line);
    std::stringstream symbolstream {line};
    getline(symbolstream, symbol_, del);
    getline(symbolstream, symbol_, del);

    // Hardcode skip of 2 lines after header 
    // TODO: get rid of hardcode

    getline(file, line);

    // Main parsing loop
    size_t lineNumber {3};
    while(getline(file, line)) {
        lineNumber++;
        try {
            events_.push_back(parseLine(line));
        } catch (const std::exception& e) {
            throw std::runtime_error(
        "Line " + std::to_string(lineNumber) + ": " + e.what()
        );
        }
    }
}

// Return the next marketevent in the stream of data as long as there is one
optional<OHLCV> EagerCSVFeed::next() {
    if (cursor_ >= events_.size()) {
        return std::nullopt;
    }
    return events_[cursor_++];
}

// Split string line into vector of stirngs.
vector<string> EagerCSVFeed::splitCSVLine(const string& line) {
    vector<string> split_csv;
    std::stringstream ss{line};
    std::string temp;
    const char del = ',';

    while (getline(ss, temp, del)) {
        split_csv.push_back(temp);
    }

    return split_csv;
}

// Handle Date
std::chrono::year_month_day EagerCSVFeed::parseDate(const string& s) {
    std::tm tm = {};
    std::stringstream ss {s};
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (!ss) std::cout << "Date parsing failed" << std::endl;

    return std::chrono::year_month_day{
        std::chrono::year{tm.tm_year + 1900},
        std::chrono::month{static_cast<unsigned>(tm.tm_mon + 1)},
        std::chrono::day{static_cast<unsigned>(tm.tm_mday)}
    };
}

OHLCV EagerCSVFeed::parseLine(const string& line) {
    const vector<string> col_data = splitCSVLine(line);
    OHLCV event;
    event.date_ = parseDate(col_data[0]);
    event.open_ = std::stod(col_data[5]);
    event.high_ = std::stod(col_data[3]);
    event.low_ = std::stod(col_data[4]);
    event.close_ = std::stod(col_data[1]);
    event.volume_=std::stod(col_data[6]);

    return event;
}


void EagerCSVFeed::validateHeader(const string& line) {
    // PROBLEM: Silently fails if "Close" column is not present because "Close" is a substring of "Adj Close"
    const vector<string> validHeaders = {"Adj Close", "Close", "High", "Low", "Open", "Volume"}; 
    for (const string& col: validHeaders) {
        if (line.find(col) == std::string::npos) {
            throw std::runtime_error (col + " column not found!");
        }
    }
}
