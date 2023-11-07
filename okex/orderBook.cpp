#include"orderBook.hpp"
using namespace okex;

orderbook::orderbook(){
}

orderbook::orderbook(std::string sym, processor * _processor){
    processor_ = _processor;
    symbol = sym;
}


void orderbook::udpateBook(const char * data){
    Document document;
    document.Parse(data);
    int64_t timestamp = std::stoll(document["data"][0]["ts"].GetString());
    bool bl = document["data"][0]["bids"].IsArray();
    if(bl){
        for(auto& level : document["data"][0]["bids"].GetArray()){
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
    bl = document["data"][0]["asks"].IsArray();

    if(bl){
        for(auto& level : document["data"][0]["asks"].GetArray()){
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
    processor_->pushOrderBook("okex" , symbol , timestamp, array);

    // std::cout<<symbol<<" "<<"bid:"<<book[0].price<<"|ask:"<<book[10].price<<std::endl;


}