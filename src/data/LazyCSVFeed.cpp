#include <string>
#include <optional>
#include <fstream>
#include <sstream>
#include <limits>
#include <charconv>
#include <chrono>
#include "LazyCSVFeed.h"
#include "Event.h"
#include "fast_float/fast_float.h"


LazyCSVFeed::LazyCSVFeed(const std::string& csvPath) {
    file_.open(csvPath);
    if (!file_.is_open()) throw std::runtime_error("Could not open CSV: " + csvPath);
    // Ignore first header row
    file_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

timestamp LazyCSVFeed::parse_time(std::string_view sv) {
    auto to_int = [](std::string_view s) {
        int v = 0;
        for (char c : s) v = v * 10 + (c - '0');
        return v;
    };

    int y  = to_int(sv.substr(0, 4));
    int mo = to_int(sv.substr(5, 2));
    int d  = to_int(sv.substr(8, 2));
    int h  = to_int(sv.substr(11, 2));
    int mi = to_int(sv.substr(14, 2));
    int s  = to_int(sv.substr(17, 2));

    auto date = std::chrono::year{y} / mo / d;         // year_month_day
    std::chrono::sys_days days_tp = std::chrono::sys_days{date};
    std::chrono::sys_seconds tp = days_tp + std::chrono::hours{h} + std::chrono::minutes{mi} + std::chrono::seconds{s};
    return std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp);
}

std::optional<MarketEvent> LazyCSVFeed::next() {


    std::string line;
    if (!std::getline(file_, line) || line.empty()) return std::nullopt;  
    auto split_line = split_s(line, ',');

    Bar bar;
    bar.symbol = "SPY";
    bar.ts = parse_time(split_line[0]);
    auto open_res = fast_float::from_chars(split_line[1].data(), split_line[1].data() + split_line[1].size(), bar.open);
    auto high_res = fast_float::from_chars(split_line[2].data(), split_line[2].data() + split_line[2].size(), bar.high);
    auto low_res = fast_float::from_chars(split_line[3].data(), split_line[3].data() + split_line[3].size(), bar.low);
    auto close_res = fast_float::from_chars(split_line[4].data(), split_line[4].data() + split_line[4].size(), bar.close);
    auto volume_res = fast_float::from_chars(split_line[5].data(), split_line[5].data() + split_line[5].size(), bar.volume);

    return bar;

}

std::vector<std::string_view> LazyCSVFeed::split_s(std::string_view str, char del) {
    std::vector<std::string_view> result;
    size_t start = 0;
    size_t end = str.find(del);
    while (end != std::string_view::npos) {
        // Extract string_views split by comma delimiter
        result.emplace_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(del, start);
    }
    // Get final substr after no comma del is found
    result.emplace_back(str.substr(start));
    return result;
}




