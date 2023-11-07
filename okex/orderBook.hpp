

#include<map>
#include<iostream>
#include<string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include"../processor.hpp"

using namespace std;
using namespace rapidjson;

namespace okex{

struct bookLevel{
    double price= 0;
    double qty = 0;
}__attribute((packed));

class orderbook{

    public:
    orderbook();
    orderbook(std::string , processor * _processor);
    void udpateBook(const char * data);
    void fireEvent(int64_t timestamp);
    std::string symbol;
    std::map<double , double> bidLevel;
    std::map<double , double> askLevel;

    bookLevel book[20];

    processor * processor_;

};

}