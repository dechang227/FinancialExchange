//
//  feedevent_unittest.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 4/28/22.
//

#include <stdio.h>
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"
#include "../FinancialExchange/include/feedevent.hpp"

namespace fe::feedevent{
    namespace{
        TEST(TradeEventTest, ToString){
            const TradeEvent t{
                .price = price::Price4("12.34"),
                .quantity = 100
            };
            EXPECT_EQ(t.to_str(), std::string("{\"price\":\"12.34\",\"quantity\":100,\"type\":\"TRADE\"}"));
        }
    
        TEST(ActionEventTest, ToString){
            const ActionEvent a("ADD", price::Price4("12.34"), 100);
            EXPECT_EQ(a.to_str(), std::string("{\"action\":\"ADD\",\"price\":\"12.34\",\"quantity\":100}"));
        }
    
        TEST(DepthUpdateEventsTest, ToString){
            DepthUpdateEvents due;
            std::shared_ptr<ActionEvent> b1 = std::make_shared<ActionEvent>("ADD", price::Price4("12.34"), 100);
            std::shared_ptr<ActionEvent> b2 = std::make_shared<ActionEvent>("DELETE", price::Price4("12.34"), 100);
            std::shared_ptr<ActionEvent> a1 = std::make_shared<ActionEvent>("ADD", price::Price4("12.34"), 100);
            std::shared_ptr<ActionEvent> a2 = std::make_shared<ActionEvent>("MODIFY", price::Price4("12.34"), 100);
            due.bid_events.push_back(b1);
            due.bid_events.push_back(b2);
            due.ask_events.push_back(a1);
            due.ask_events.push_back(a2);
            EXPECT_EQ(due.type, "DEPTH_UPDATE");
            EXPECT_EQ(due.to_str(), std::string("{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"12.34\\\",\\\"quantity\\\":100}\",\
\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"12.34\\\",\\\"quantity\\\":100}\"],\
\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"12.34\\\",\\\"quantity\\\":100}\",\
\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"12.34\\\",\\\"quantity\\\":100}\"],\
\"type\":\"DEPTH_UPDATE\"}"));
        }
    
        TEST(FeedEventsTest, ToString){
            DepthUpdateEvents due;
            std::shared_ptr<ActionEvent> b1 = std::make_shared<ActionEvent>("ADD", price::Price4("12.34"), 100);
            std::shared_ptr<ActionEvent> b2 = std::make_shared<ActionEvent>("DELETE", price::Price4("12.34"), 100);
            std::shared_ptr<ActionEvent> a1 = std::make_shared<ActionEvent>("ADD", price::Price4("12.34"), 100);
            std::shared_ptr<ActionEvent> a2 = std::make_shared<ActionEvent>("MODIFY", price::Price4("12.34"), 100);
            due.bid_events.push_back(b1);
            due.bid_events.push_back(b2);
            due.ask_events.push_back(a1);
            due.ask_events.push_back(a2);
            
            
            const TradeEvent t{
                .price = price::Price4("12.34"),
                .quantity = 100
            };
            
            FeedEvents fe;
            fe.symbol = "AAPL";
            fe.timestamp = 0;
            fe.trade_events.push_back(t);
            fe.depth_update_events = due;
            
            EXPECT_EQ(fe.to_str(), std::string("{\"symbol\":\"AAPL\",\"timestamp\":0}\n\
{\"price\":\"12.34\",\"quantity\":100,\"type\":\"TRADE\"}\n\
{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"12.34\\\",\\\"quantity\\\":100}\",\
\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"12.34\\\",\\\"quantity\\\":100}\"],\
\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"12.34\\\",\\\"quantity\\\":100}\",\
\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"12.34\\\",\\\"quantity\\\":100}\"],\
\"type\":\"DEPTH_UPDATE\"}"));
        }
    }
}
