//
//  orderbook_unittest.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 4/10/22.
//

#include <stdio.h>
#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "nlohmann/json.hpp"
#include "../FinancialExchange/include/jsonparser.hpp"
#include "../FinancialExchange/include/orderbook.hpp"

namespace fe::orderbook{
    namespace{
    using json = nlohmann::json;
    
        TEST(OrderBookTest, Actions){
            price::Price4 p1("12.01");
            const json order1_json = {
                {"time", 0},
                {"order_id", 1},
                {"symbol", "test"},
                {"price", "12.01"},
                {"quantity", 100},
                {"order_type", "LIMIT"},
                {"order_side", "BUY"},
                {"order_status", "NEW"}
            };
//            std::shared_ptr<order::Order> o1(new order::Order({0, 1, "test", p1, 100, order::OrderType::LIMIT, order::OrderSide::BUY, order::OrderStatus::NEW}));
            std::shared_ptr<order::Order> o1 = std::make_shared<order::Order>(order1_json);
            
            const json order2_json = {
                {"time", 0},
                {"order_id", 2},
                {"symbol", "test"},
                {"price", "12.01"},
                {"quantity", 100},
                {"order_type", "LIMIT"},
                {"order_side", "BUY"},
                {"order_status", "NEW"}
            };
//            std::shared_ptr<order::Order> o2(new order::Order({0, 2, "test", p1, 100, order::OrderType::LIMIT, order::OrderSide::BUY, order::OrderStatus::NEW}));
            std::shared_ptr<order::Order> o2 = std::make_shared<order::Order>(order2_json);
            BidOrderBook bob;
            EXPECT_EQ(bob.GetLimit(price::Price4("23.02")), nullptr);

            bob.InsertOrder(o1);
            bob.InsertOrder(o2);
            EXPECT_TRUE(bob.ContainsOrder(1));
            EXPECT_TRUE(bob.ContainsOrder(2));
            EXPECT_NE(bob.TopLimit(), nullptr);
            bob.RemoveOrder(o1);
            EXPECT_FALSE(bob.ContainsOrder(1));
            EXPECT_TRUE(bob.ContainsOrder(2));
            EXPECT_NE(bob.TopLimit(), nullptr);
            bob.RemoveOrder(o2);
            EXPECT_FALSE(bob.ContainsOrder(1));
            EXPECT_FALSE(bob.ContainsOrder(2));
            EXPECT_EQ(bob.TopLimit(), nullptr);
        }
    }
}
