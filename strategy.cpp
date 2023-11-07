#include "strategy.hpp"
#include <ctime>
#include <chrono>

using namespace Strategy;
std::map<std::string,map<std::string,int> > positionOrders;

strategy::strategy()
{
	std::ifstream file("ArbitrageSymbols.txt");
	std::string line;
	while(getline(file,line)){
		std::stringstream ss(line);
		std::string symbol1,symbol2;
		getline(ss,symbol1,',');
		getline(ss,symbol2,',');
		std::cout<<symbol1<<" "<<symbol2<<"\n";
		portfolio[symbol1]=symbol2;
		portfolio[symbol2]=symbol1;
	}
}

void strategy::updateData(std::string symbol,int64_t time, bookLevel book[20],std::string exchange)
{
	if(portfolio.find(symbol)==portfolio.end())
		return;
	bestBid[symbol]=book[0].price;
	bestAsk[symbol]=book[10].price;
	bestBidQuantity[symbol]=book[0].qty;
	bestAskQuantity[symbol]=book[10].qty;
	std::string pairedSymbol=portfolio[symbol];
	auto currentTime = std::chrono::system_clock::now();
	std::time_t timenow = std::chrono::system_clock::to_time_t(currentTime);

    // Use std::ctime to convert the time_t to a human-readable string
    //std::cout << "Current system time: " << std::ctime(&time);

	//std::cout<<symbol<<" "<<bestBid[symbol]<<" "<<bestAsk[symbol]<<" "<<pairedSymbol<<" "<<bestBid[pairedSymbol]<<" "<<bestAsk[pairedSymbol]<<"\n";
	double arbitrage=0;
	double tradableQuantity=0;
	double fairPrice=0;
	double percent=0;
	if(bestBid[symbol]>bestAsk[pairedSymbol] && bestBid[symbol]!=0 && bestAsk[pairedSymbol]!=0)
	{
		arbitrage=bestBid[symbol]-bestAsk[pairedSymbol];
		fairPrice=(bestBid[symbol]+bestAsk[pairedSymbol])/2;
		percent=(arbitrage/fairPrice)*100;
		if(bestBidQuantity[symbol]<=bestAskQuantity[pairedSymbol])
			tradableQuantity=bestBidQuantity[symbol];
		else
			tradableQuantity=bestAskQuantity[pairedSymbol];
		if(percent>=0.2 && percent < 5)
		{
		std::cout<<"sell "<<symbol<<" and buy "<<pairedSymbol<<" at arb is "<<arbitrage<<" at time "<<time<<" at quantity "<<tradableQuantity<<" percent " <<percent<<" "<<std::ctime(&timenow)<<"\n";
		if(positionOrders[pairedSymbol][symbol]==0 && positionOrders[symbol][pairedSymbol]==0)
		{
			positionOrders[symbol][pairedSymbol]=1;
		}
		else if (positionOrders[pairedSymbol][symbol]==1 && positionOrders[symbol][pairedSymbol]==0)
		{
			std::cout<<"merged "<<symbol<<" "<<pairedSymbol<<" "<<std::ctime(&timenow)<<"\n";
			positionOrders[symbol][pairedSymbol]=1;
			positionOrders[pairedSymbol][symbol]=0;
		}
		}
		else if (percent<0.2 && positionOrders[pairedSymbol][symbol]==1 && positionOrders[symbol][pairedSymbol]==0)
		{
			std::cout<<"merged "<<symbol<<" "<<pairedSymbol<<" "<<std::ctime(&timenow)<<"\n";
		}
	}
	else if(bestBid[pairedSymbol]>bestAsk[symbol] && bestBid[pairedSymbol]!=0 && bestAsk[symbol]!=0)
	{
		fairPrice=(bestBid[pairedSymbol]+bestAsk[symbol])/2;
		arbitrage=bestBid[pairedSymbol]-bestAsk[symbol];
		percent=(arbitrage/fairPrice)*100;
		if(bestBidQuantity[pairedSymbol]<=bestAskQuantity[symbol])
				tradableQuantity=bestBidQuantity[pairedSymbol];
		else
			tradableQuantity=bestAskQuantity[symbol];
		if(percent>=0.2 &&  percent < 5)
		{
		std::cout<<"sell "<<pairedSymbol<<" and buy "<<symbol<<" at arb is "<<arbitrage<<" at time "<<time<<" at quantity "<<tradableQuantity<<" percent "<<percent<<" "<<std::ctime(&timenow)<<"\n";
		if(positionOrders[pairedSymbol][symbol]==0 && positionOrders[symbol][pairedSymbol]==0)
                {
                        positionOrders[pairedSymbol][symbol]=1;
                }
                else if (positionOrders[pairedSymbol][symbol]==0 && positionOrders[symbol][pairedSymbol]==1)
                {
                        std::cout<<"merged "<<symbol<<" "<<pairedSymbol<<" "<<std::ctime(&timenow)<<"\n";
			positionOrders[pairedSymbol][symbol]=1;
			positionOrders[symbol][pairedSymbol]=0;
                }
		}
		else if(percent<0.2 && positionOrders[pairedSymbol][symbol]==0 && positionOrders[symbol][pairedSymbol]==1)
		{
			std::cout<<"merged "<<symbol<<" "<<pairedSymbol<<" "<<std::ctime(&timenow)<<"\n";
		}
	}
}
	


