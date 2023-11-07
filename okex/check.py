import requests
import json

url = "https://eapi.binance.com/eapi/v1/exchangeInfo"  # Use the futures API endpoint
response = requests.get(url)

if response.status_code == 200:
    data = response.json()
    # Pretty-print the entire exchangeInfo response
    print(json.dumps(data, indent=4))
else:
    print("Failed to retrieve exchangeInfo.")
