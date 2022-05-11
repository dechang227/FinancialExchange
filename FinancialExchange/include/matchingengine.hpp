//
//  matchingengine.hpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/12/22.
//

#ifndef matchingengine_hpp
#define matchingengine_hpp

#include <stdio.h>
#include <string>
#include <unordered_map>
#include "price4.hpp"
#include "order.hpp"
#include "orderbook.hpp"



namespace fe::matchingengine{

    using namespace fe::price;
    using namespace fe::order;
    using namespace fe::orderbook;

    class MatchingEngine{
    public:
        MatchingEngine();
        MatchingEngine(const MatchingEngine &) = delete;
        MatchingEngine(MatchingEngine &&) = delete;
        ~MatchingEngine() = default;
        
        void StartEngine();
        void StopEngine();
        std::string ValidateOrder(std::shared_ptr<Order> order);
        std::string Cancel(std::shared_ptr<Order> order);
        std::string Process(std::shared_ptr<Order> order);

    private:
        std::unordered_map<std::string, BidOrderBook> bid_order_books_;
        std::unordered_map<std::string, AskOrderBook> ask_order_books_;
        std::unordered_map<int64_t, std::shared_ptr<Order>> order_id_map_;

    }; //namespace fe::matchingengine
    }


#endif /* matchingengine_hpp */
