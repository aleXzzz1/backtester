import yfinance as yf
import os

def main():
    # Prompt for tickers (space- or comma-separated), e.g. "TSLA AAPL MSFT"
    raw = input("Enter ticker(s), separated by spaces or commas: ").strip()
    tickers = [t.strip().upper() for t in raw.replace(",", " ").split() if t.strip()]
    if not tickers:
        print("No tickers entered. Exiting.")
        return

    # Optional start date; press Enter to default to 2019-01-01
    start = input("Start date (YYYY-MM-DD) [default 2019-01-01]: ").strip()
    if not start:
        start = "2019-01-01"

    os.makedirs("data", exist_ok=True)

    for ticker in tickers:
        # auto_adjust=False keeps both 'Adj Close' and raw 'Close',
        # matching the header your EagerCSVFeed expects.
        df = yf.download(ticker, start=start, auto_adjust=False)

        if df.empty:
            print(f"  No data for {ticker} - skipping (check the symbol/date).")
            continue

        out_path = os.path.join("data", f"{ticker}.csv")
        df.to_csv(out_path)
        print(f"  Saved {out_path} ({len(df)} rows)")

if __name__ == "__main__":
    main()