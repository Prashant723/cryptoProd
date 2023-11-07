import requests

# API Endpoint URL for fetching spot trading pairs
url = 'https://www.okx.com/api/v5/public/instruments?instType=SPOT'

# Make the API request
response = requests.get(url)

# Check for a successful response (status code 200)
if response.status_code == 200:
    data = response.json()
    # Extract the list of symbols or trading pairs
    symbols = [instrument['instId'] for instrument in data['data']]
    
    # Print each symbol on a new line
    for symbol in symbols:
        print(symbol)
else:
    print(f"Error: {response.status_code} - {response.text}")

