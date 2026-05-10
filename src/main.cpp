#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include "ohlcv.h"

Date parseDate(std::string date_p) {
    Date date;
    std::stringstream ss(date_p);
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

std::vector<OHLCV> parseLines(std::ifstream& file, int numLines) {
    std::string line;
    std::vector<OHLCV> vec;
    getline(file, line);
    getline(file, line);
    getline(file, line);
    // hardcoded skip of first three lines

    for (int i = 0; i < numLines; i++) {
        getline(file, line);
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

        OHLCV obj = OHLCV(open, high, low, close, volume, date);
        vec.push_back(obj);
    }

    return vec;

}

void printLines(const std::vector<OHLCV>& vec) {
    for (const OHLCV& obj : vec) {
        Date date = obj.getDate();
        std::string d;
        d += std::to_string(date.year);
        d += "-";
        d += std::to_string(date.month);
        d += "-";
        d += std::to_string(date.day);

        std::cout << d << ": open=" << obj.getOpen() << " high=" << obj.getHigh() << " low=" << obj.getLow() << " close=" << obj.getClose()
        << " volume=" << obj.getVolume() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        // Print usage and exit
        std::cerr << "Usage: " << argv[0] << "<csv_file> <num_lines>" << std::endl;
        return 1;
    }

    std::ifstream file{argv[1]};
    int numLines = std::stoi(argv[2]);

    if (!file) {
        // Print an error and exit
        std::cerr << "Uh oh, TSLA.csv could not be opened for reading!\n";
        return 1;
    }

    std::vector<OHLCV> vec = parseLines(file, numLines);

    printLines(vec);

    return 0;


}