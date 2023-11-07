
import requests

url = "https://api.binance.com/api/v3/exchangeInfo"
response = requests.get(url)

if response.status_code == 200:
    data = response.json()
    spot_symbols = [symbol["symbol"] for symbol in data["symbols"] if symbol["status"] == "TRADING"]

    for symbol in spot_symbols:
        print(symbol)
else:
    print("Failed to retrieve spot symbols.")
