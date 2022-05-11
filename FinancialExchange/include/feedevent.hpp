//
//  feedevent.hpp
//  FinancialExchange
//
//  Created by Dechang Chen on 4/22/22.
//

#ifndef feedevent_hpp
#define feedevent_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "price4.hpp"

namespace fe::feedevent{
    struct TradeEvent{
        std::string type = "TRADE";
        fe::price::Price4 price;
        int64_t quantity;
        
        std::string to_str() const;
    };
    
    struct ActionEvent{
        ActionEvent(const std::string a, const fe::price::Price4 &p, int64_t q):action(a), price(p), quantity(q){}
        
        std::string action;
        fe::price::Price4 price;
        int64_t quantity;
        
        std::string to_str() const;
    };

      
//    struct AddEvent : public ActionEvent{
//
//    };
//
//    struct DeleteEvent : public ActionEvent{
//
//    };
//
//    struct ModifyEvent : public ActionEvent{
//
//    };

    struct DepthUpdateEvents{
        std::string type="DEPTH_UPDATE";
        std::vector<std::shared_ptr<ActionEvent>> bid_events;
        std::vector<std::shared_ptr<ActionEvent>> ask_events;
            
        std::string to_str() const;
    };

    struct FeedEvents{
        std::vector<TradeEvent> trade_events;
        DepthUpdateEvents depth_update_events;
        int64_t timestamp=0;
        std::string symbol;
        
        std::string to_str() const;
    };
}


#endif /* feedevent_hpp */
