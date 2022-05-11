//
//  feedevent.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 4/22/22.
//

#include <string>
#include "nlohmann/json.hpp"
#include "feedevent.hpp"

namespace fe::feedevent{
    using nlohmann::json;
    
    std::string TradeEvent::to_str() const{
        json j;
        j["type"] = this->type;
        j["price"] = this->price.to_str();
        j["quantity"]= this->quantity;
        return j.dump();
    }

    std::string ActionEvent::to_str() const{
        json j;
        j["action"] = this->action;
        j["price"] = this->price.to_str();
        j["quantity"]= this->quantity;
        return j.dump();
    }

    std::string DepthUpdateEvents::to_str() const{
        json j;
        j["type"] = this->type;
        j["bid"] = {};
        j["ask"] = {};
        for (const auto &event : this->bid_events){
            if (event == nullptr)
                continue;
            else{
                j["bid"].push_back(event->to_str());
            }
        }
        for (const auto & event : this->ask_events){
            if (event == nullptr)
                continue;
            else{
                j["ask"].push_back(event->to_str());
            }
        }

        return j.dump();
    }

    std::string FeedEvents::to_str() const{
        json j;
        std::string context;
        
        j["symbol"] = this->symbol;
        j["timestamp"] = this->timestamp;
        
        context += j.dump() + "\n";
        for (const auto &event : this->trade_events){
            context += event.to_str()+"\n";
        }
        context += this->depth_update_events.to_str();
        return context;
    }
}
