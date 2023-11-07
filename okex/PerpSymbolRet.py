import requests

# API Endpoint URL for fetching perpetual contract trading pairs
url = 'https://www.okx.com/api/v5/public/instruments?instType=FUTURES'

# Make the API request
response = requests.get(url)

# Check for a successful response (status code 200)
if response.status_code == 200:
    data = response.json()
    # Extract the list of symbols for perpetual contracts
    symbols = [instrument['instId'] for instrument in data['data']]
    
    # Print each symbol on a new line
    for symbol in symbols:
        print(symbol)
else:
    print(f"Error: {response.status_code} - {response.text}")
