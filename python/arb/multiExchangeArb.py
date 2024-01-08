from MdBinance import MdBinance
from MdOkx import MdOkx
from multiprocessing import Process, Queue
#mdBinance=MdBinance()
mdBinance=MdOkx()
dataQueue = Queue()
#binanceDataReadProcess=Process(target=MdBinance.readDataAndPushToQueue,args=(mdBinance,dataQueue))
binanceDataReadProcess=Process(target=MdOkx.readDataAndPushToQueue,args=(mdBinance,dataQueue))
binanceDataReadProcess.start()
while True:
    print(dataQueue.get())
