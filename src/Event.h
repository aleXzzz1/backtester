#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <string>
#include <variant>
#include <chrono>

enum class Direction { LONG, FLAT, SHORT };
using timestamp = std::chrono::system_clock::time_point;

struct Bar {std::string symbol; timestamp ts; 
            double open, high, low, close, volume;};
struct Tick {std::string symbol; timestamp ts; double price;};
struct SignalEvent {std::string symbol; timestamp ts; Direction direction;};
struct OrderEvent {std::string symbol; timestamp ts; double volume;};
struct FillEvent {std::string symbol; timestamp ts; double price, volume, total_cost, commission;};

using MarketEvent = std::variant<Bar, Tick>;
using Event = std::variant<MarketEvent, SignalEvent, OrderEvent, FillEvent>;


inline double execution_price(const Tick& t) { return t.price; }   // bar fills happen at next open
inline double mark_price(const Tick& t)      { return t.price; }  // value positions at close
inline auto   event_time(const Tick& t)      { return t.ts; }
inline std::string event_symbol(const Tick& t) { return t.symbol; }

inline double execution_price(const Bar& bar) { return bar.open; }   // bar fills happen at next open
inline double mark_price(const Bar& bar)      { return bar.close; }  // value positions at close
inline auto   event_time(const Bar& bar)      { return bar.ts; }
inline std::string event_symbol(const Bar& bar)  { return bar.symbol; }


inline std::string event_symbol(const MarketEvent& m) {
    return std::visit([](const auto& e) { return event_symbol(e); }, m);
}
inline auto event_time(const MarketEvent& m) {
    return std::visit([](const auto& e) -> timestamp { return event_time(e); }, m);
}
inline double execution_price(const MarketEvent& m) {
    return std::visit([](const auto& e) { return execution_price(e); }, m);
}
inline double mark_price(const MarketEvent& m) {
    return std::visit([](const auto& e) { return mark_price(e); }, m);
}



#endif