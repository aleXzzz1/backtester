#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Date {
    int year, month, day;
};

class OHLCV {
    private:
        double m_open;
        double m_high;
        double m_low;
        double m_close;
        int m_volume;
        Date m_date;

    public:
        OHLCV(double open, double high, double low, double close, double volume, Date date) {
            if (low > high) std::cerr << "WARNING: Invalid low price!";
            m_open = open;
            m_high = high;
            m_low = low;
            m_close = close;
            m_volume = volume;
            m_date = date;
        }

        double getOpen() const {
            return m_open;
        }
        
        double getHigh() const {
            return m_high;
        }

        double getLow() const {
            return m_low;
        }

        double getClose() const {
            return m_close;
        }

        int getVolume() const {
            return m_volume;
        }

        Date getDate() {
            return m_date;
        }

};

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
        std::cout << high << std::endl;

        // low
        getline(ss, t, del);
        double low = std::stod(t);

        // open
        getline(ss, t, del);
        double open = std::stod(t);

        // volume
        getline(ss, t);
        int volume = std::stoi(t);

        OHLCV obj = OHLCV(open, high, low, close, volume, date);
        vec.push_back(obj);
    }

    return vec;

}

void printLines(const std::vector<OHLCV>& vec) {
    for (OHLCV obj : vec) {
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
        std::cout << "Usage: " << argv[0] << "3" << std::endl;
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