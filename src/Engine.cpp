#include "Engine.h"

using std::vector;

void Engine::run() {

    // Initialize queue with first market event
    if (auto first = feed_->next()) {
        eventQ.push(*first);
    }

    while (!eventQ.empty()) {
        Event ev = std::move(eventQ.front());
        eventQ.pop();

        std::visit([this](const auto& e) { handle(e); }, ev);

        if (std::holds_alternative<MarketEvent>(ev)) { // Check to see if the event we processed was a MarketEvent; feed in next MarketEvent if so.
            if (auto next = feed_->next()) {   // check to see that next MarketEvent From feed is not std::nullopt
                eventQ.push(*next);
            }
        }
    }
}

void Engine::handle(const MarketEvent& event) {
    ctx_.update(event);
    // Fill pending orders first
    auto it = pending_orders.find(event_symbol(event));
    if (it != pending_orders.end()) {
        for (const OrderEvent& o : it->second) {
            FillEvent fill = broker_.fill(o, ctx_);
            eventQ.push(fill);
        }
    }

    // Feed event to strategy for signal
    auto signals = strgy_->on_event(event, ctx_);
    for (const SignalEvent& s : signals) {
        eventQ.push(s);
    }
}

void Engine::handle(const SignalEvent& signal) {
    eventQ.push(portfolio_.consider(signal, ctx_));
}

void Engine::handle(const OrderEvent& order) {
    eventQ.push(broker_.fill(order, ctx_));
}

void Engine::handle(const FillEvent& fill) {
    portfolio_.apply(fill, ctx_);
}

