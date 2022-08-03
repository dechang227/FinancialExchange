//
//  main.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/6/22.
//
#define HAS_STRING_VIEW 0
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <memory>
#include <mutex>
#include <thread>

//#include "glog/logging.h"
#include "date/tz.h"
#include "nlohmann/json.hpp"
#include "../include/price4.hpp"
#include "../include/order.hpp"
#include "../include/jsonparser.hpp"
#include "../include/feedevent.hpp"
#include "../include/matchingengine.hpp"
#include "../include/datapublisher.hpp"


using json = nlohmann::json;
using namespace date;
using namespace std::chrono_literals;
using namespace std::chrono;
using namespace fe::matchingengine;
using namespace fe::price;
using namespace fe::order;
using namespace fe::orderbook;
using namespace fe::publisher;



std::mutex g_feed_mu;
std::condition_variable g_feed_cond;
std::deque<std::string> g_feed_buffer;
//constexpr unsigned int maxBufferSize = 50;

std::mutex g_market_mu;
bool g_market_starts = false;
std::condition_variable g_market_cond;


const char timeZone[] = "America/New_York";
const std::string root_path = "/Users/dchen/Projects/FinancialExchange/FinancialExchange";
//std::string NY = "America/New_York";


void MarketStatusChecker(){
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(60));
        auto time_now = system_clock::now();
        const std::string now_str = date::format("%Y-%m-%d", make_zoned(timeZone, time_now));
//        LOG(INFO) << "Checking market status at " << now_str << "...";
        std::unique_lock<std::mutex> locker(g_market_mu);
        g_market_starts = (now_str == "09:00");
        locker.unlock();
        g_market_cond.notify_one();
    }
}

void RunMatchingEngine(json config_json){
    auto time_now = system_clock::now();
    const std::string current_date = date::format("%Y-%m-%d", make_zoned(timeZone, time_now));
    const std::string previous_date = date::format("%Y-%m-%d", make_zoned(timeZone, time_now - date::days{1}));
    
    const std::string orders_to_process = root_path + "/data/orders.json";
    const std::string previous_date_umatched_orders = root_path + "/" + previous_date + "/orders.json";
    const std::string current_date_umatched_orders = root_path + "/" + current_date + "/orders.json";
    
    std::cout << "Market start: " << current_date << std::endl;
//    LOG(INFO) << "Start processing for date: " << current_date;
    
    MatchingEngine matching_engine(config_json["lot_size"], config_json["tick_size_rules"]);
    std::string pre_market_process_message = matching_engine.PreMarketProcess(previous_date_umatched_orders);
    
//    LOG(INFO) << pre_market_process_message;
    
    std::vector<Order> orders = ReadOrdersFromPath(orders_to_process);
    
    for (const Order &order : orders){
        std::cout << order.symbol << std::endl;
        auto order_time = date::sys_seconds{seconds(order.time)};
        const std::string order_time_str = date::format("%H:%M", make_zoned(timeZone, order_time));
        std::cout << order_time_str << std::endl;
        if (order_time_str > "16:00")
            {
              return;
            }
        std::unique_lock<std::mutex> locker(g_feed_mu);

        const std::string process_message = matching_engine.Process(std::make_shared<Order>(order));
        g_feed_buffer.push_back(process_message);
        locker.unlock();
        g_feed_cond.notify_one();
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    const std::string after_market_process_message = matching_engine.AfterMarketProcess(current_date_umatched_orders);
    std::cout <<  "Market end: " << after_market_process_message << std::endl;
};


void RunPublisher()
{
    DataPublisher data_publisher;
    while (true) {
        std::unique_lock<std::mutex> locker(g_feed_mu);
        g_feed_cond.wait(locker, []()
                         { return g_feed_buffer.size() > 0; });

        std::string events = g_feed_buffer.front();
        data_publisher.Publish(events);
        g_feed_buffer.pop_front();

        locker.unlock();
        // g_feed_cond.notify_one();

        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
}


void Run(json config) {
  std::thread t1(RunMatchingEngine, config);
  std::thread t2(RunPublisher);

  t1.join();
  t2.join();
}

void MatchAndPublish(json config) {
  while (true)
  {
    std::unique_lock<std::mutex> locker(g_market_mu);
    g_market_cond.wait(locker, []()
                       { return g_market_starts; });
    std::cout << "Market starts!" << std::endl;
    g_market_starts = false;
    locker.unlock();
    Run(config);
  }
}


int main(int argc, const char * argv[]) {
    std::ifstream config_file(root_path+"/data/config.json");
    json config_json;
    config_file >> config_json;
    
    std::thread market_status_thread(MarketStatusChecker);
    std::thread match_publish_thread(MatchAndPublish, config_json);

    market_status_thread.join();
    match_publish_thread.join();

    return 0;
    
//    std::string test_time = date::format("%H:%M", make_zoned(timeZone, sys_seconds{seconds(1205812558)}));
//    std::cout << test_time << '\n';
//    bool test_time_comp = test_time > "16:00";
//    std::cout << test_time_comp << '\n';
//
//    auto t = make_zoned(timeZone, system_clock::now());
//
//    std::cout << date::format("%Y-%m-%d",t) << '\n';
//
//
//    auto now_t = date::clock_cast_detail::system_clock::now();
//    std::cout << now_t << std::endl;
//
////    auto ny_now_t = make_zoned("Asia/Tehran", local_days{December/30/1978} + 12h +  1min);
////    auto ny_now_t = make_zoned("America/New_York", date::clock_cast_detail::system_clock::now());
////    std::cout << ny_now_t << std::endl;
//    // insert code here...
//    fe::price::Price4 p;
//    fe::price::Price4 p4_sample("1230");
//    p = p4_sample;
//    std::cout << p.to_str() << std::endl;
////    fe::order::Limit* l1 = new fe::order::Limit(p4_sample);
//    std::shared_ptr<fe::order::Limit> l1(new fe::order::Limit(p4_sample));
//    std::shared_ptr<fe::order::Order> o1(new fe::order::Order({0, 1, "test", p4_sample, 100, fe::order::OrderType::LIMIT, fe::order::OrderSide::BUY, fe::order::OrderStatus::NEW}));
//    std::shared_ptr<fe::order::OrderBookEntry> obe1 = std::make_shared<fe::order::OrderBookEntry>(o1, l1);
//    l1->head_entry = obe1;
//    l1->tail_entry = obe1;
//    if (l1->GetOrderSide()==fe::order::OrderSide::BUY){
//        std::cout << "Hello, World!\n" << "BUY" << "\n";
//    }
//
//
//    const json order_json = {
//        {"time", 1},
//        {"order_id", 1},
//        {"symbol", "test"},
//        {"price", "12.01"},
//        {"quantity", 100},
//        {"order_type", "LIMIT"},
//        {"order_side", "BUY"},
//        {"order_status", "NEW"},
//        {"time_in_force", "GTC"}
//    };
//    order_json["price"].get<fe::price::Price4>();
//    fe::order::Order o = order_json.get<fe::order::Order>();
////    std::cout << "n" << n << std::endl;
//
//
//    fe::feedevent::DepthUpdateEvents due;
//    std::shared_ptr<fe::feedevent::ActionEvent> b1 = std::make_shared<fe::feedevent::ActionEvent>(std::string("ADD"), fe::price::Price4("12.34"), 100);
//    due.bid_events.push_back(b1);
//
////    std::cout << due.to_str() << std::endl;
//    std::unordered_map<std::string, BidOrderBook> test_order_books;
//    test_order_books["test"];
//
//
//    if(config_json["tick_size_"].empty()){
//        std::cout << "Check" << std::endl;
//    }
//    std::cout << config_json["tick_size_"] << std::endl;
//    std::cout << config_json["tick_size_rules"][0]["to_price"] << std::endl;
//
//    MatchingEngine me(config_json["lot_size"], config_json["tick_size_rules"]);
//    std::shared_ptr<Order> new_order = std::make_shared<Order>(order_json);
//
//    std::string event_message = me.Process(new_order);
//    std::cout << "Event: " << event_message << std::endl;
    

//    return 0;
}
