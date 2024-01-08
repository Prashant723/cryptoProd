#make a request
import requests
from websocket import create_connection
from multiprocessing import Process, Queue
from time import sleep
import json
class MdBinance:
    def __init__(self) -> None:
        #declare binance websocket url
        socket_url="wss://fstream.binance.com/ws"
        #declare binance rest api url
        public_url="https://fapi.binance.com/fapi/v1"
        self.ws = create_connection(socket_url)
        #declare the parameters
        params = {'symbol': 'BTCUSDT'}
        #subscribe to book for a symbol
        data='{"method": "SUBSCRIBE","params":["btcusdt@bookTicker"],"id": 1}'
        self.ws.send(data)
    def readData(self):
        while True:
            data = self.ws.recv()
            print(data)
    def readDataAndPushToQueue(self,queue):
        while True:
            data = self.ws.recv()
            data = json.loads(data)
            print(data,type(data))
            if ("e" in data) and (data["e"] == "bookTicker"):
                finalData={}
                finalData["exchange"]="binance"
                finalData["updateTime"]=data["T"]
                finalData["symbol"]=data["s"]
                finalData["bids"]=[]
                finalData["asks"]=[]
                finalData["bids"].append([data["b"],data["B"]])
                finalData["asks"].append([data["a"],data["A"]])
                print(finalData)
                queue.put(data)
if __name__=="__main__":
    md=MdBinance()
    q = Queue()
    p=Process(target=MdBinance.readDataAndPushToQueue,args=(md,q))
    p.start()
    sleep(30)
    exit(0)
    while True:
        print(q.get())
    
    #md.readData()