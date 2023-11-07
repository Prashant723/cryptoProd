#include"processor.hpp"

processor::processor(){
}
processor::processor(strategy * _strategy){
    strategy_ = _strategy;

    // std::thread t1(&processor::processOrderBook, this);
    // t1.detach();
}
processor::~processor(){

}

void processor::processOrderBook(){

    // while(1){

    //     if(!_bookQueue.empty()){
    //         // _obMutex.lock();
    //         bookbuf tmp;
    //         tmp = _bookQueue.front();
    //         _bookQueue.pop();
    //         std::cout<<tmp.exchange<<" "<<tmp.symbol<<" "<<tmp.book[0].price<<" "<<tmp.book[10].price<<std::endl;
    //         // _obMutex.unlock();

    //     }
    // }
}


void processor::pushOrderBook(std::string exchange , std::string symbol, int64_t timestamp , char * ob){
    memset(book , ' ' , 20*sizeof(bookLevel));
    memcpy(book , ob , 20*sizeof(bookLevel));
    strategy_->updateData(symbol , timestamp , book, exchange);
    // _obMutex.lock();
    // _bookQueue.push(tmp);
    // _obMutex.unlock();
    
}
