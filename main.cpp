#include<iostream>
#include "binance/md.hpp"
#include "okex/md.hpp"
#include"processor.hpp"
using namespace std;




int main(){
    auto strategy_ = new strategy;
    auto processor_ = new processor(strategy_);
    auto binanceMd = new binance::websocket_endpoint(processor_);
    auto okexMd = new okex::websocket_endpoint(processor_);
    sleep(100000);
    return 0;
}