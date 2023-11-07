#import requests

#url = "https://eapi.binance.com/eapi/v1/exchangeInfo"
#response = requests.get(url)

#if response.status_code == 200:
    #data = response.json()
    #symbols = [symbol["symbol"] for symbol in data["symbols"]]

    #for symbol in symbols:
     #   print("Symbol:", symbol)
#else:
 #   print("Failed to retrieve symbols.")

import requests
import json

url = "https://eapi.binance.com/eapi/v1/exchangeInfo"
response = requests.get(url)

if response.status_code == 200:
    data = response.json()
    # Print the entire JSON response for inspection
    print(json.dumps(data, indent=4))
else:
    print("Failed to retrieve data from the API.")
