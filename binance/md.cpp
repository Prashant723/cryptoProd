#include"md.hpp"
#include<fstream>
#include<vector>
#include<map>
using namespace binance;

session::session(){}

session::session(processor * _processor){
    processor_ = _processor;
    initOrderBookMap();
}

void session::initOrderBookMap(){
}
std::vector<std::string> session::getSubscriptionStrings(const std::string& subscriptionType) {
    std::ifstream symbolFile("binanceSymbols.txt");
    std::string symbol;
    std::vector<std::string> subscriptionStrings;
    int symbolsNo=0;
    int limit = 200;
    std::string baseUri;
    if (subscriptionType == "Spot_") {
        baseUri = "wss://stream.binance.com:9443/stream?streams=";
        limit = 500;
    } else if (subscriptionType == "Perpetual_") {
        baseUri = "wss://fstream.binance.com/stream?streams=";
    } 
    else {
        // Handle invalid subscriptionType
        return subscriptionStrings;
    }

    std::string currentSubscription = baseUri;

    if (symbolFile.is_open()) {
        while (std::getline(symbolFile, symbol)) {
            if (symbol.find(subscriptionType) == 0) {
                // Symbol matches the requested type
                transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower);
                currentSubscription += symbol.substr(subscriptionType.length()) + "@depth/";
                symbolsNo=symbolsNo+1;
		//mapSymbolsConnection[subscriptionType][symbol]=connection;

                // If the current subscription string exceeds 500 symbols, store it and start a new one
                if (symbolsNo%limit==0) {
                    currentSubscription.pop_back(); // Remove the trailing '/'
                    subscriptionStrings.push_back(currentSubscription);
                    currentSubscription = baseUri;
		    //connection=connection+1;
                }
            }
        }

        // Remove the trailing '/' and store the last subscription string
        if (symbolsNo%limit!=0) {
            currentSubscription.pop_back();
            subscriptionStrings.push_back(currentSubscription);
	   }
    }

    return subscriptionStrings;
}

/*std::pair<std::string, std::string> session::getSubscriptionString() {
    std::ifstream symbolFile("binanceSymbols.txt");
    std::string symbol;
    std::string spotUri = "wss://stream.binance.com:9443/stream?streams=";
    std::string perpetualUri = "wss://fstream.binance.com/stream?streams=";

     if (symbolFile.is_open()) {
         while (std::getline(symbolFile, symbol)) {
            //  std::cout<<symbol<<std::endl;
             if (symbol.find("Spot_") == 0) {
                 // Spot symbol
                transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower); 
                 spotUri += symbol.substr(5) + "@depth/";
             } else if (symbol.find("Perpetual_") == 0) {
                 // Perpetual symbol
                transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower); 
                 perpetualUri += symbol.substr(10) + "@depth/";
             }
         }

         // Remove the trailing '/' for both URIs
         spotUri.pop_back();
         perpetualUri.pop_back();
     }

    return std::make_pair(spotUri, perpetualUri);
}*/

connection_metadata::connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri, session* _session)
    : m_id(id)
    , m_hdl(hdl)
    , m_status("Connecting")
    , m_uri(uri)
    , m_server("N/A")
{
    session_ = _session;
}

void connection_metadata::on_open(client * c, websocketpp::connection_hdl hdl) {
    m_status = "Open";

    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
}

void connection_metadata::on_fail(client * c, websocketpp::connection_hdl hdl) {
    m_status = "Failed";

    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
    m_error_reason = con->get_ec().message();
}

void connection_metadata::on_close(client * c, websocketpp::connection_hdl hdl) {
    m_status = "Closed";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    std::stringstream s;
    s << "close code: " << con->get_remote_close_code() << " (" 
        << websocketpp::close::status::get_string(con->get_remote_close_code()) 
        << "), close reason: " << con->get_remote_close_reason();
    m_error_reason = s.str();
}

void connection_metadata::on_message(int id, client::message_ptr msg) {
    
    if (msg->get_opcode() == websocketpp::frame::opcode::text) {
        // std::cout<<msg->get_payload()<<std::endl;
    } else {
        std::cout<<websocketpp::utility::to_hex(msg->get_payload())<<std::endl;
        return;
    }

    Document document;
    document.Parse(msg->get_payload().c_str());
    std::string symbol = document["data"]["s"].GetString();
    if(session_->mapSymbolsConnection[id] == "Spot" ){
        symbol = "SPOT_" + symbol;
    }
    else {
        symbol = "PERPETUAL_" + symbol;
    }    
    if(session_->orderBookMap.find(symbol) == session_->orderBookMap.end()){
            session_->orderBookMap[symbol] = new orderbook(symbol , session_->processor_);
    }
    else{
        session_->orderBookMap[symbol]->udpateBook(msg->get_payload().c_str());

    }

    

}

websocketpp::connection_hdl connection_metadata::get_hdl() const {
    return m_hdl;
}

int connection_metadata::get_id() const {
    return m_id;
}

std::string connection_metadata::get_status() const {
    return m_status;
}

websocket_endpoint::websocket_endpoint (){}
websocket_endpoint::websocket_endpoint (processor * _processor) : m_next_id(0) {
    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint.set_access_channels(websocketpp::log::alevel::all ^ (websocketpp::log::alevel::frame_payload | websocketpp::log::alevel::frame_header));
    m_endpoint.clear_error_channels(websocketpp::log::elevel::all);
    m_endpoint.set_tls_init_handler([](websocketpp::connection_hdl) {
            return websocketpp::lib::make_shared<boost::asio::ssl::context>(
                boost::asio::ssl::context::sslv23);
        });
    m_endpoint.init_asio();
    m_endpoint.start_perpetual();

    m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint);
    processor_ = _processor; 
    session_ = new session(processor_);
    connect();
}

websocket_endpoint::~websocket_endpoint() {
    m_endpoint.stop_perpetual();
    
    for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it) {
        if (it->second->get_status() != "Open") {
            // Only close open connections
            continue;
        }
        
        std::cout << "> Closing connection " << it->second->get_id() << std::endl;
        
        websocketpp::lib::error_code ec;
        m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
        if (ec) {
            std::cout << "> Error closing connection " << it->second->get_id() << ": "  
                        << ec.message() << std::endl;
        }
    }
    
    m_thread->join();
}

size_t websocket_endpoint::writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}




/*std::string websocket_endpoint::getSubscriptionString(){

    std::string substring = "wss://stream.binance.com:9443/ws/btcusdt@depth";
    return substring;
    std::vector<std::string> symVector;
    std::ifstream symbolFile ("binanceSymbols.txt");
    std::string symbol;
    if ( symbolFile.is_open() ) {
        while ( symbolFile ) {
            std::getline (symbolFile, symbol);
            symVector.push_back(symbol);
        } 
    }
    std::string url = "https://api.binance.com/api/v3/exchangeInfo";
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cout << "Failed to initialize cURL" << std::endl;
        return "fu";
    }
    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, "TRUE");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "cURL failed: " << curl_easy_strerror(res) << std::endl;
        return "fu";
    }
    std::cout<<response;
    curl_easy_cleanup(curl);

    return response;

}*/

int websocket_endpoint::connect() {
    
    std::string uri;
    //std::pair<std::string, std::string> uris = session_->getSubscriptionString();

    //std::string spotUri = uris.first; // Store the spot URI
    //std::string perpetualUri = uris.second; // Store the perpetual URI
    std::vector<std::string> spotUris = session_->getSubscriptionStrings("Spot_");
    std::vector<std::string> perpetualUris = session_->getSubscriptionStrings("Perpetual_");

    for(int i = 0 ;i < spotUris.size()+perpetualUris.size(); i++){

        if(i < spotUris.size()){
            uri = spotUris[i];
	    session_->mapSymbolsConnection[i]="Spot";
        }
        else{
            uri = perpetualUris[i-spotUris.size()];
	    session_->mapSymbolsConnection[i]="Perpetual";
        }
        
        websocketpp::lib::error_code ec;
        // std::cout<<uri<<std::endl;
        client::connection_ptr con = m_endpoint.get_connection(uri.c_str(), ec);

        if (ec) {
            std::cout << "> Connect initialization error: " << ec.message() << std::endl;
            return -1;
        }

        int new_id = m_next_id++;
        connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(new_id, con->get_handle(), uri , session_ );
        m_connection_list[new_id] = metadata_ptr;

        con->set_open_handler(websocketpp::lib::bind(
            &connection_metadata::on_open,
            metadata_ptr,
            &m_endpoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_fail_handler(websocketpp::lib::bind(
            &connection_metadata::on_fail,
            metadata_ptr,
            &m_endpoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_close_handler(websocketpp::lib::bind(
            &connection_metadata::on_close,
            metadata_ptr,
            &m_endpoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_message_handler(websocketpp::lib::bind(
            &connection_metadata::on_message,
            metadata_ptr,
            new_id,
            websocketpp::lib::placeholders::_2
        ));

        m_endpoint.connect(con);
    }

    return m_next_id;
}

void websocket_endpoint::close(int id, websocketpp::close::status::value code, std::string reason) {
    websocketpp::lib::error_code ec;
    
    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }
    
    m_endpoint.close(metadata_it->second->get_hdl(), code, reason, ec);
    if (ec) {
        std::cout << "> Error initiating close: " << ec.message() << std::endl;
    }
}

void websocket_endpoint::send(int id, std::string message) {
    websocketpp::lib::error_code ec;
    
    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }
    
    m_endpoint.send(metadata_it->second->get_hdl(), message, websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "> Error sending message: " << ec.message() << std::endl;
        return;
    }
}

connection_metadata::ptr websocket_endpoint::get_metadata(int id) const {
    con_list::const_iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        return connection_metadata::ptr();
    } else {
        return metadata_it->second;
    }
}
