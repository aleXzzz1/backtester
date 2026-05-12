#include "stats.h"
#include <queue>

std::vector<std::optional<double>> closing_SMA(const std::vector<OHLCV>& vec) {
    std::vector<std::optional<double>> sma_vec;
    
    double rsum = 0.0;     // rolling sum
    std::queue<double> q;  // queue to keep track of oldest close
    q.push(rsum);

    // Skip first 19 rows for 20-day simple moving average (SMA)
    for (int i = 0; i < 19; i++) {
        sma_vec[i] = std::nullopt;
        q.push(vec[i].m_close);
        rsum += vec[i].m_close;
    }
    
    for (int i = 19; i < vec.size(); i++) {
        double front = q.front();
        rsum -= front;              // subtract oldest element from rolling sum
        rsum += vec[i].m_close;     // add newest element to rolling sum
        q.push(vec[i].m_close);     // push newest element to back of queue
        sma_vec[i] = (rsum / 20.0);
    }
}