#include<iostream>
#include<fstream>
#include<vector>
#include<map>

std::map<std::string,std::map<std::string,int> > mapSymbolsConnection;
int connection=0;

std::vector<std::string> getSubscriptionStrings(const std::string& subscriptionType) {
    std::ifstream symbolFile("binanceSymbols.txt");
    std::string symbol;
    std::vector<std::string> subscriptionStrings;
    int symbolsNo=0;

    std::string baseUri;
    if (subscriptionType == "Spot_") {
        baseUri = "wss://stream.binance.com:9443/stream?streams=";
    } else if (subscriptionType == "perpetual") {
        baseUri = "wss://fstream.binance.com/stream?streams=";
    } else {
        // Handle invalid subscriptionType
        return subscriptionStrings;
    }

    std::string currentSubscription = baseUri;

    if (symbolFile.is_open()) {
        while (std::getline(symbolFile, symbol)) {
            if (symbol.find(subscriptionType) == 0) {
                // Symbol matches the requested type
                //transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower);
                currentSubscription += symbol.substr(subscriptionType.length()) + "@depth/";
		symbolsNo=symbolsNo+1;
		mapSymbolsConnection[subscriptionType][symbol]=connection;

                // If the current subscription string exceeds 500 symbols, store it and start a new one
                if (symbolsNo%500==0) {
                    currentSubscription.pop_back(); // Remove the trailing '/'
                    subscriptionStrings.push_back(currentSubscription);
                    currentSubscription = baseUri;
		    connection=connection+1;
                }
            }
        }

        // Remove the trailing '/' and store the last subscription string
        if (!currentSubscription.empty()) {
            currentSubscription.pop_back();
            subscriptionStrings.push_back(currentSubscription);
        }
    }

    return subscriptionStrings;
}

/*std::pair<std::string, std::string> getSubscriptionString() {
    std::ifstream symbolFile("binanceSymbols.txt");
    std::string symbol;
    std::string spotUri = "wss://stream.binance.com:9443/stream?streams=";
    std::string perpetualUri = "wss://fstream.binance.com/stream?streams=";

    if (symbolFile.is_open()) {
             std::cout<<"zxcv1 file open"<<std::endl;

         while (std::getline(symbolFile, symbol)) {
             std::cout<<symbol<<std::endl;
            if (symbol.find("Spot_") == 0) {
                 // Spot symbol
                 spotUri += symbol.substr(5) + "@depth/";
             } else if (symbol.find("Perpetual_") == 0) {
                 // Perpetual symbol
                 perpetualUri += symbol.substr(10) + "@depth/";
             }
         }

        // Remove the trailing '/' for both URIs
         spotUri.pop_back();
        perpetualUri.pop_back();
     }

    return std::make_pair(spotUri, perpetualUri);
}*/

int main()
{
	std::vector<std::string> p=getSubscriptionStrings("Spot_");
	for(int i=0;i<p.size();i++)
	{
		std::cout<<p[i]<<std::endl;
	}
	std::cout<<p.size()<<std::endl;
	return 0;
}
