#include "Engine.h"
#include "MarketEvent.h"

using std::vector;

template <typename Event>
void Engine<Event>::run() {
    const std::string& s = feed_->get_symbol();
    while (auto event = feed_->next()) {
        ctx_.update(*event, s);
        vector<Fill> fills = broker_.fill(orderBuffer_, execution_price(*event)); // Fullfill orders made at on_bar[t] at time [t+1]
        portfolio_.apply(fills, ctx_);  // Update portfolio based on fills given by broker and current market context
        orderBuffer_.clear();
        vector<Signal> new_signals = strgy_->on_event(*event, ctx_);
        vector<Order> new_orders = portfolio_.consider(new_signals, ctx_);
        orderBuffer_.insert(orderBuffer_.end(),
                            new_orders.begin(), new_orders.end());
    }
    if (event == std::nullopt) {
        portfolio_.close();
    }
}

template class Engine<OHLCV>;