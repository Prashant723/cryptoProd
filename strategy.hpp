#pragma once
#include <fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<map>
using namespace std;


namespace Strategy{

	struct bookLevel{
		double price= 0;
		double qty = 0;
	}__attribute((packed));


	class strategy{
		public:
			strategy();

			void updateData(std::string symbol,int64_t time,bookLevel book[20],std::string exchange);
		private:
			std::map<std::string,std::string> portfolio;
			std::map<std::string,double> bestBid;
			std::map<std::string,double> bestAsk;
			std::map<std::string,double> bestBidQuantity;
			std::map<std::string,double> bestAskQuantity;
	};
}
