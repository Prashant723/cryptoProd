#include"orderBook.hpp"
using namespace binance;
orderbook::orderbook(){
}

orderbook::orderbook(std::string sym , processor * _processor){
    symbol = sym;
    processor_ = _processor;
}


void orderbook::udpateBook(const char * data){
    Document document;
    document.Parse(data);
    if(document["data"].HasMember("T")){
        timestamp = document["data"]["T"].GetDouble();
    }

    bool bl = document["data"]["b"].IsArray();
    if(bl){
        for(auto& level : document["data"]["b"].GetArray()){
                double price = std::stold(level[0].GetString());
                double qty = std::stold(level[1].GetString());
                if(qty == 0){
                    if(bidLevel.find(price) != bidLevel.end()){
                        bidLevel.erase(price);
                    }
                }
                else{
                    bidLevel[price] = qty;
                }
        }
    }
    bl = document["data"]["a"].IsArray();

    if(bl){
        for(auto& level : document["data"]["a"].GetArray()){
                double price = std::stold(level[0].GetString());
                double qty = std::stold(level[1].GetString());
                if(qty == 0){
                    if(askLevel.find(price) != askLevel.end()){
                        askLevel.erase(price);
                    }
                }
                else{
                    askLevel[price] = qty;
                }
        }
    }

    fireEvent(timestamp);
}

void orderbook::fireEvent(int64_t timestamp){
    int index = 0;
    for (auto it = bidLevel.rbegin(); it != bidLevel.rend(); it++) {
        if(index == 10){
            break;
        }
        book[index].price = it->first;
        book[index].qty = it->second;
        index++;
    }

    for(auto it = askLevel.begin(); it != askLevel.end(); it++){
        if(index == 20){
            break;
        }
        
        book[index].price = it->first;
        book[index].qty = it->second;
        index++;
    }

        char array[1024];
    memset(array , ' ', 1024);
    memcpy(&array , book, 20*sizeof(bookLevel));
    processor_->pushOrderBook("binance" , symbol , timestamp, array);


}