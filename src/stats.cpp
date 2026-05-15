#include "stats.h"
#include <queue>
#include <deque>
#include <cmath>

std::vector<std::optional<double>> closing_SMA(const std::vector<OHLCV>& prices, int window) {
    std::vector<std::optional<double>> sma_vec;
    
    double rsum = 0.0;     // rolling sum
    std::queue<double> q;  // queue to keep track of oldest close
    q.push(rsum);

    // Skip first n-1 rows for n-day simple moving average (SMA)
    for (int i = 0; i < window - 1; i++) {
        sma_vec.push_back(std::nullopt);
        q.push(prices[i].m_close);
        rsum += prices[i].m_close;
    }

    
    for (int i = window - 1; i < prices.size(); i++) {
        double front = q.front();
        q.pop();
        rsum -= front;              // subtract oldest element from rolling sum
        rsum += prices[i].m_close;     // add newest element to rolling sum
        q.push(prices[i].m_close);     // push newest element to back of queue
        sma_vec.push_back(std::optional<double>{rsum / window});
    }

    return sma_vec;
}



std::vector<std::optional<double>> closing_EMA(const std::vector<OHLCV>& prices, int window) {
    std::vector<std::optional<double>> ema_vec;

    double rsum = 0.0;
    double multiplier = (2.0 / (window+1.0));

    // Skip first n-1 rows for n-day simple moving average (SMA)
    for (int i = 0; i < window - 1; i++) {
        rsum += prices[i].m_close;
        ema_vec.push_back(std::nullopt);
    }

    rsum += prices[window-1].m_close;
    ema_vec.push_back(std::optional<double>{rsum / window}); // SMA for first 20 days as first EMA
    for (int i = window; i < prices.size(); i++) {
        ema_vec.push_back(prices[i].m_close * multiplier + *ema_vec[i-1]*(1 - multiplier));
    }

    return ema_vec;

}

double stddev(const std::deque<double>& vals, double sma) {
    double acc = 0.0;
    for (const double& val : vals) {
        acc += (val - sma)*(val - sma);
    }
    return sqrt(acc / (vals.size()));
}

std::vector<std::optional<BollingerBand>> BollBands(const std::vector<OHLCV>& prices, int window) {
    std::vector<std::optional<double>> sma_vec = closing_SMA(prices, window);
    std::vector<std::optional<BollingerBand>> bb_vec;
    
    std::deque<double> dq;  // queue to keep track of oldest close

    // skip the first window - 1 rows until we have an SMA.
    for (int i = 0; i < window - 1; i++) {
        bb_vec.push_back(std::nullopt);
        dq.push_back(prices[i].m_close);
    }

    for (int i = window - 1; i < prices.size(); i++) {
        dq.push_back(prices[i].m_close);
        double sma = *sma_vec[i];
        double sd = stddev(dq, sma);
        BollingerBand bb{(sma + 2*sd), sma, (sma - 2*sd)};
        bb_vec.push_back(bb);
        dq.pop_front();
    }

    return bb_vec;


}

