```mermaid
flowchart LR
    %% ===== Data sources =====
    subgraph DATA["Data Source"]
        CSV[("Historical CSV(yfinance)")]
        FUTURE["WebSocket / Tick API(planned)"]:::planned
    end

    %% ===== Feed layer =====
    subgraph FEED["Data Feed"]
        DF["DataFeed"]:::abstract
        ECF["EagerCSVFeednext() → MarketEvent"]
        TF["TickFeed (planned)"]:::planned
        DF -.implements.-> ECF
        DF -.implements.-> TF
    end

    CSV --> ECF
    FUTURE -.-> TF

    %% ===== Event loop =====
    subgraph ENGINE["Engine (event loop)"]
        Q[/"Event Queue 
        std::variant<Market, Signal, Order, Fill>"/]
        DISPATCH{{"std::visit → handle(...)"}}
        Q --> DISPATCH
    end

    ECF -- MarketEvent --> Q

    %% ===== Handlers =====
    subgraph HANDLERS["Per-event handlers"]
        HM["handle(MarketEvent)
Update ctx & equity curve, run strategy"]
        HS["handle(SignalEvent)
        portfolio sizes order"]
        HO["handle(OrderEvent)
        broker fills"]
        HF["handle(FillEvent)
        portfolio applies"]
    end

    DISPATCH --> HM
    DISPATCH --> HS
    DISPATCH --> HO
    DISPATCH --> HF

    %% ===== Components used by handlers =====
    subgraph CORE["Engine state"]
        CTX[(MarketContext
        Per symbol price history)]
        STRAT["MACrossover : Strategy"]:::abstract
        PORT[(Portfolio, 
        Positions, Equity Curve)]
        BROK[Broker
        Slippage + commission]
    end

    HM --> CTX
    HM --> STRAT
    HS --> PORT
    HO --> BROK
    HF --> PORT

    %% ===== Re-emission back to queue =====
    STRAT -. SignalEvent .-> Q
    PORT -. OrderEvent .-> Q
    BROK -. FillEvent .-> Q

    %% ===== Output =====
    PORT --> ANALYTICS
    subgraph ANALYTICS["Analytics (post-run)"]
        REPORT["PerformanceReportreturns, Sharpe, drawdown,alpha vs buy-and-hold"]
        OUT_CONSOLE["Console output"]
        OUT_CSV["CSV output(planned)"]:::planned
        OUT_PLOTS["Plots(planned)"]:::planned
        REPORT --> OUT_CONSOLE
        REPORT -.-> OUT_CSV
        REPORT -.-> OUT_PLOTS
    end

    %% ===== Styling =====
    classDef planned stroke-dasharray: 5 5,fill:#2a2a2a,color:#aaa
    classDef abstract font-style:italic,fill:#1f3a5f
    ```
