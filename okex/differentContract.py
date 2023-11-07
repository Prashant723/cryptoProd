import requests

url = "https://fapi.binance.com/fapi/v1/exchangeInfo"  # Use the futures API endpoint
response = requests.get(url)

if response.status_code == 200:
    data = response.json()
    contract_types = set(symbol["contractType"] for symbol in data["symbols"])

    if contract_types:
        print("Available contract types:")
        for contract_type in contract_types:
            print(contract_type)
    else:
        print("No contract types found in the data.")
else:
    print("Failed to retrieve contract types.")
