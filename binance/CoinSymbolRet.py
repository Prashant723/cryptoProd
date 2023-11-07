import requests

url = "https://fapi.binance.com/fapi/v1/exchangeInfo"  # Use the futures API endpoint

response = requests.get(url)

if response.status_code == 200:
    data = response.json()
    coin_symbols = [symbol["symbol"] for symbol in data["symbols"] if symbol["contractType"] == "COIN-M" or symbol["contractType"] == "COIN-ONECOIN"]

    for symbol in coin_symbols:
        print(symbol)
else:
    print("Failed to retrieve coin symbols.")

