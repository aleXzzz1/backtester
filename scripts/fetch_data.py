import yfinance as yf
import os

tickers = ["TSLA"]
os.makedirs("data", exist_ok=True)

for ticker in tickers:
    df = yf.download(ticker, start="2019-01-01", auto_adjust=False)
    df.to_csv(f"data/{ticker}.csv")
    print(f"Saved {ticker}.csv ({len(df)} rows)")