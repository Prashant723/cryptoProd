#import ccxt
import pandas as pd
from asyncio import get_event_loop,run
import ccxt.pro as ccxtpro
import datetime
from time import sleep
okex_fut=ccxtpro.okx()
binance_fut=ccxtpro.binanceusdm()
async def load_markets(exchange):
    await exchange.load_markets()
    print(exchange.symbols)
print(datetime.datetime.now())
#run(load_markets(binance_fut))
#run(load_markets(okex_fut))
print(datetime.datetime.now())
#watch order book for symbol
symbol='BTC/USDT'
symbol='BTC/USDT:USDT'
async def main():
    while True:
        #orderbook = await okex_fut.watch_order_book(symbol)
        orderbook = await binance_fut.watch_order_book(symbol)
        print(orderbook['asks'][0], orderbook['bids'][0])
run(main())
async def close_market(exchange):
    await exchange.close()
run(close_market(okex_fut))
from time import sleep
sleep(10)