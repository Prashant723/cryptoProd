#pragma once

#include<iostream>
#include<memory>
#include<boost/lockfree/queue.hpp>
#include<thread>
#include<queue>
#include<mutex>
#include"strategy.hpp"

using namespace Strategy;

struct bookbuf{
    std::string exchange;
    std::string symbol;
    Strategy::bookLevel book_[20];
    int64_t timestamp;

};


class processor{
    public:
        processor();
        processor(strategy * _strategy);
        ~processor();
        void processOrderBook();
        void pushOrderBook(std::string exchange , std::string symbol, int64_t timestamp , char * ob);
        std::mutex _obMutex;
        std::queue<bookbuf> _bookQueue;
        strategy * strategy_;

        Strategy::bookLevel book[20];
        
};