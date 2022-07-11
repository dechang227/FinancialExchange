//
//  matchingengin_test.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 7/10/22.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"
#include "../FinancialExchange/include/jsonparser.hpp"
#include "../FinancialExchange/include/matchingengine.hpp"

namespace fe::matchingengine{
    namespace{
        using namespace fe::order;
        using namespace fe::price;
        using namespace fe::matchingengine;
        using json = nlohmann::json;

        TEST(MatchingEngineTest, ProcessLimitOrders){
            std::ifstream config_file("/Users/dchen/Projects/FinancialExchange/FinancialExchange/data/config.json");
            json config_json;
            config_file >> config_json;
            
            MatchingEngine engine(config_json["lot_size"], config_json["tick_size_rules"]);
            const json order_data = {
                {"time", 10},
                {"order_id", 1},
                {"symbol", "AAPL"},
                {"price", "124.40"},
                {"quantity", 100},
                {"order_type", "LIMIT"},
                {"order_side", "BUY"},
                {"order_status", "NEW"},
                {"time_in_force", "DAY"}
            };

            std::shared_ptr<Order> order = std::make_shared<Order>(order_data);
            const auto &event = engine.Process(order);
            EXPECT_EQ(event,std::string("\{\"symbol\":\"AAPL\",\"timestamp\":10\}\n\{\"ask\":null,\"bid\":[\"\{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"124.4\\\",\\\"quantity\\\":100\}\"],\"type\":\"DEPTH_UPDATE\"\}"));
        }
    }
}
