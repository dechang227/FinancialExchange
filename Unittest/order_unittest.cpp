//
//  order_unittest.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 4/9/22.
//

#include <stdio.h>
#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../FinancialExchange/include/order.hpp"


namespace fe::order{
    namespace {
        TEST(LimitTest, IsEmpty){
            price::Price4 p1("12.01");
            std::shared_ptr<Limit> l1(new Limit(p1));
            EXPECT_EQ(true, l1->IsEmpty());

//            struct Order o1({0, 1, "test", p1, 100, OrderType::LIMIT, OrderSide::BUY, OrderStatus::NEW});
            std::shared_ptr<Order> o1(new Order({0, 1, "test", p1, 100, OrderType::LIMIT, OrderSide::BUY, OrderStatus::NEW}));
            std::shared_ptr<OrderBookEntry> obe1 = std::make_shared<OrderBookEntry>(o1, l1);
            l1->head_entry = obe1;
            l1->tail_entry = obe1;
            EXPECT_EQ(false, l1->IsEmpty());
        }
        TEST(LimitTest, GetOrderSide){
            price::Price4 p1("12.01");
            std::shared_ptr<Limit> l1(new Limit(p1));
            
            EXPECT_EQ(OrderSide::UNKNOWN, l1->GetOrderSide());
             
//            struct Order o1({0, 1, "test", p1, 100, OrderType::LIMIT, OrderSide::BUY, OrderStatus::NEW});
            std::shared_ptr<Order> o1(new Order({0, 1, "test", p1, 100, OrderType::LIMIT, OrderSide::BUY, OrderStatus::NEW}));

            OrderBookEntry obe(o1, l1);
            EXPECT_EQ(OrderSide::BUY, obe.current_order->order_side);

            std::shared_ptr<OrderBookEntry> obe1 = std::make_shared<OrderBookEntry>(o1, l1);
            EXPECT_EQ(OrderSide::BUY, obe1->current_order->order_side);
            l1->head_entry = obe1;
            l1->tail_entry = obe1;
            EXPECT_EQ(OrderSide::BUY, l1->GetOrderSide());
        }
        TEST(LimitTest, GetOrderCounts){
            price::Price4 p1("12.01");
            std::shared_ptr<Limit> l1(new Limit(p1));
            
            EXPECT_EQ(0, l1->GetOrderCounts());
             
    //            struct Order o1({0, 1, "test", p1, 100, OrderType::LIMIT, OrderSide::BUY, OrderStatus::NEW});
            std::shared_ptr<Order> o1(new Order({0, 1, "test", p1, 100, OrderType::LIMIT, OrderSide::BUY, OrderStatus::NEW}));
            std::shared_ptr<OrderBookEntry> obe1 = std::make_shared<OrderBookEntry>(o1, l1);
            l1->head_entry = obe1;
            l1->tail_entry = obe1;
            EXPECT_EQ(1, l1->GetOrderCounts());
        }
        TEST(LimitTest, GetSize){
            price::Price4 p1("12.01");
            std::shared_ptr<Limit> l1(new Limit(p1));
            
            EXPECT_EQ(0, l1->GetSize());
             
            std::shared_ptr<Order> o1(new Order({0, 1, "test", p1, 100, OrderType::LIMIT, OrderSide::BUY, OrderStatus::NEW}));
            std::shared_ptr<OrderBookEntry> obe1 = std::make_shared<OrderBookEntry>(o1, l1);
            l1->head_entry = obe1;
            l1->tail_entry = obe1;
            EXPECT_EQ(100, l1->GetSize());
        }
    }
}
