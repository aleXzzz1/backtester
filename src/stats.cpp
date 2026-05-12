#include "stats.h"
#include <queue>

std::vector<std::optional<double>> closing_SMA(const std::vector<OHLCV>& vec, int n) {
    std::vector<std::optional<double>> sma_vec;
    
    double rsum = 0.0;     // rolling sum
    std::queue<double> q;  // queue to keep track of oldest close
    q.push(rsum);

    // Skip first n-1 rows for n-day simple moving average (SMA)
    for (int i = 0; i < n - 1; i++) {
        sma_vec.push_back(std::nullopt);
        q.push(vec[i].m_close);
        rsum += vec[i].m_close;
    }
    
    for (int i = n - 1; i < vec.size(); i++) {
        double front = q.front();
        q.pop();
        rsum -= front;              // subtract oldest element from rolling sum
        rsum += vec[i].m_close;     // add newest element to rolling sum
        q.push(vec[i].m_close);     // push newest element to back of queue
        sma_vec.push_back(std::optional<double>{rsum / n});
    }

    return sma_vec;
}

std::vector<std::optional<double>>closing_EMA(const std::vector<OHLCV>& vec, int n) {
    std::vector<std::optional<double>> ema_vec;

    double rsum = 0.0;
    double multiplier = (2.0 / (n+1.0));

    // Skip first n-1 rows for n-day simple moving average (SMA)
    for (int i = 0; i < n - 1; i++) {
        rsum += vec[i].m_close;
        ema_vec.push_back(std::nullopt);
    }

    rsum += vec[n-1].m_close;
    ema_vec.push_back(std::optional<double>{rsum / n}); // SMA for first 20 days as first EMA
    for (int i = n; i < vec.size(); i++) {
        ema_vec.push_back(vec[i].m_close * multiplier + *ema_vec[i-1]*(1 - multiplier));
    }

    return ema_vec;
}

