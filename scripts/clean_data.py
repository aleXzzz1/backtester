import pandas as pd

# Load your file
df = pd.read_csv("data/spy_cleaned_v2.csv", parse_dates=["date"])
df["day"] = df["date"].dt.date
daily = df.groupby("day")["date"].agg(["min", "max", "count"])
daily["first_time"] = daily["min"].dt.time
daily["last_time"] = daily["max"].dt.time

# Show where the session-start time changes
daily["start_changed"] = daily["first_time"] != daily["first_time"].shift()
print(daily[daily["start_changed"]][["first_time", "last_time", "count"]])

# # 1. RTH filter — removes all extended-hours bars across the whole file
# rth = df.set_index("date").between_time("07:30", "14:00").reset_index()

# # 2. dedupe on timestamp
# rth = rth.drop_duplicates(subset=["date"], keep="first").sort_values("date")

# # 3. OHLC sanity
# bad = (rth["high"] < rth["low"]) | (rth["high"] < rth["open"]) | \
#       (rth["high"] < rth["close"]) | (rth[["open","high","low","close"]] <= 0).any(axis=1)
# print(f"Dropping {bad.sum()} malformed OHLC rows")
# rth = rth[~bad]

# # 4. write engine schema
# rth["date_str"] = rth["date"].dt.strftime("%Y-%m-%d %H:%M:%S")
# assert (rth["date_str"].str.len() == 19).all()
# out = rth[["date_str","open","high","low","close","volume"]]
# out.columns = ["date","open","high","low","close","volume"]
# out.to_csv("data/spy_cleaned_v2.csv", index=False)
# print(f"Wrote {len(out)} bars, {rth['date'].dt.date.nunique()} trading days")