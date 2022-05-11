//
//  main.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/6/22.
//

#include <iostream>
#include "nlohmann/json.hpp"
#include "../include/price4.hpp"
#include "../include/order.hpp"
#include "../include/jsonparser.hpp"
#include "../include/feedevent.hpp"
#include "../include/matchingengine.hpp"

using json = nlohmann::json;
using namespace fe::matchingengine;
using namespace fe::price;
using namespace fe::order;
using namespace fe::orderbook;

int main(int argc, const char * argv[]) {
    // insert code here...
    fe::price::Price4 p;
    fe::price::Price4 p4_sample("1230");
    p = p4_sample;
    std::cout << p.to_str() << std::endl;
//    fe::order::Limit* l1 = new fe::order::Limit(p4_sample);
    std::shared_ptr<fe::order::Limit> l1(new fe::order::Limit(p4_sample));
    std::shared_ptr<fe::order::Order> o1(new fe::order::Order({0, 1, "test", p4_sample, 100, fe::order::OrderType::LIMIT, fe::order::OrderSide::BUY, fe::order::OrderStatus::NEW}));
    std::shared_ptr<fe::order::OrderBookEntry> obe1 = std::make_shared<fe::order::OrderBookEntry>(o1, l1);
    l1->head_entry = obe1;
    l1->tail_entry = obe1;
    if (l1->GetOrderSide()==fe::order::OrderSide::BUY){
        std::cout << "Hello, World!\n" << "BUY" << "\n";
    }
    
    
    const json order_json = {
        {"time", 0},
        {"order_id", 1},
        {"symbol", "test"},
        {"price", "12.01"},
        {"quantity", 100},
        {"order_type", "LIMIT"},
        {"order_side", "BUY"},
        {"order_status", "NEW"},
        {"time_in_force", "GTC"}
    };
    order_json["price"].get<fe::price::Price4>();
    fe::order::Order o = order_json.get<fe::order::Order>();
//    std::cout << "n" << n << std::endl;
    
    
    fe::feedevent::DepthUpdateEvents due;
    std::shared_ptr<fe::feedevent::ActionEvent> b1 = std::make_shared<fe::feedevent::ActionEvent>(std::string("ADD"), fe::price::Price4("12.34"), 100);
    due.bid_events.push_back(b1);
    
//    std::cout << due.to_str() << std::endl;
    std::unordered_map<std::string, BidOrderBook> test_order_books;
    test_order_books["test"];
    
    MatchingEngine me;
    std::shared_ptr<Order> new_order = std::make_shared<Order>(order_json);
    std::string event_message = me.Process(new_order);
    std::cout << "Event: " << event_message << std::endl;
    return 0;
}
