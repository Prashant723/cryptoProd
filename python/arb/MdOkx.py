#make a request
from multiprocessing import Process, Queue
import ccxt.pro as ccxtpro
from asyncio import get_event_loop,run
import datetime

class MdOkx:
    def __init__(self) -> None:
        self.okex_fut=ccxtpro.okx()
        self.symbol='BTC/USDT'
    def readData(self):
        async def main():
            while True:
                #orderbook = await okex_fut.watch_order_book(symbol)
                orderbook = await self.okex_fut.watch_order_book(self.symbol)
                print(orderbook)
                exit(0)
        run(main())
    def readDataAndPushToQueue(self,queue):
        self.queue=queue
        async def main():
            while True:
                orderbook = await self.okex_fut.watch_order_book(self.symbol)
                print(orderbook,orderbook.__dir__())
        run(main())

if __name__=="__main__":
    mdOkx=MdOkx()
    mdOkx.readData()
