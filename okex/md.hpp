
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <curl/curl.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "orderBook.hpp"
#include"../processor.hpp"
using namespace std;
using namespace rapidjson;

namespace okex{
    typedef websocketpp::client<websocketpp::config::asio_tls_client> client;

    class orderbook;

    class session{
        public:
            session();
            session(processor * _processor);
            void initOrderBookMap();
            unordered_map<std::string , orderbook *> orderBookMap;
            std::map<int,std::string> mapSymbolsConnection;

            //std::pair<std::string, std::string> getSubscriptionString();
            int getSubscriptionStrings();
            std::vector<std::string> subscriptionStrings;
            processor * processor_;

    };

    class connection_metadata {
    public:
        typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;

        connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri, session* _session);

        void on_open(int id, client * c , websocketpp::connection_hdl hdl);

        void on_fail(client * c, websocketpp::connection_hdl hdl);
        
        void on_close(client * c, websocketpp::connection_hdl hdl);

        void on_message(int id, client::message_ptr msg);
        
        websocketpp::connection_hdl get_hdl() const;
        
        int get_id() const;
        
        std::string get_status() const;

    private:
        int m_id;
        websocketpp::connection_hdl m_hdl;
        std::string m_status;
        std::string m_uri;
        std::string m_server;
        std::string m_error_reason;
        session * session_;



    };


    class websocket_endpoint {
    public:
        websocket_endpoint ();
        websocket_endpoint (processor * _processor);

        ~websocket_endpoint();

        int connect();

        void close(int id, websocketpp::close::status::value code, std::string reason);

        void send(int id, std::string message);

        size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data);


        connection_metadata::ptr get_metadata(int id) const;
    private:
        typedef std::map<int,connection_metadata::ptr> con_list;

        client m_endpoint;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

        con_list m_connection_list;
        int m_next_id = 0;
        session * session_;
        processor * processor_;

    };


}