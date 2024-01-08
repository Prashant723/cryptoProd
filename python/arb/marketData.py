#declare the websocket url
socket_url="wss://ws.okx.com:8443/ws/v5/public"
public_url="https://www.okx.com/api/v5/public"
#get the trading instruments
import requests
import json
#declare the headers
headers = {'Content-Type': 'application/json'}
#declare the parameters
params = {'instType': 'FUTURES'}
#make a request
response = requests.get(public_url + '/instruments', headers=headers, params=params)
#convert the response to json
data = response.json()
#print(data)
#exit(0)
#create_connection function
from websocket import create_connection

#connect to the websocket
ws = create_connection(socket_url)
data='{"op": "subscribe", "args": [{"channel":"books5","instId": "BTC-USDC-240628"}]}'
import json
ws.send(data)
while True:
    data = ws.recv()
    print(data)