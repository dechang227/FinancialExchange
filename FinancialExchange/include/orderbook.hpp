//
//  orderbook.hpp
//  FinancialExchange
//
//  Created by Dechang Chen on 4/2/22.
//

#ifndef orderbook_hpp
#define orderbook_hpp

#include <stdio.h>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "order.hpp"

namespace fe::orderbook{
    struct BidComparator{
        bool operator()(const std::shared_ptr<order::Limit> &lhs, const std::shared_ptr<order::Limit> &rhs) const
        {
            return lhs->price < rhs->price;
        }
    };

    struct AskComparator{
        bool operator()(const std::shared_ptr<order::Limit> &lhs, const std::shared_ptr<order::Limit> &rhs) const
        {
            return lhs->price > rhs->price;
        }
    };

    template<class T>
    class OrderBook{
    public:
        OrderBook() = default;
        OrderBook(const OrderBook&) = delete;
        OrderBook(OrderBook&&) = delete;
        
        virtual bool MatchOrder(std::shared_ptr<order::Order> input_order, std::shared_ptr<order::Order> target_order) const = 0;
        
        void InsertOrder(std::shared_ptr<order::Order> order){
            const auto it = price_to_limit_map_.find(order->price);
            if(it == price_to_limit_map_.end()){
                std::shared_ptr<order::Limit> base_limit = std::make_shared<order::Limit>(order->price);
                std::shared_ptr<order::OrderBookEntry> orderbook_entry = std::make_shared<order::OrderBookEntry>(order, base_limit);
                base_limit->head_entry = orderbook_entry;
                base_limit->tail_entry = orderbook_entry;
                limit_levels_.push(base_limit);
                orderbook_.insert({order->order_id, orderbook_entry});
                price_to_limit_map_.insert({order->price, base_limit});
            }
            else{
                std::shared_ptr<order::Limit> parent_limit=it->second;
                std::shared_ptr<order::OrderBookEntry> orderbook_entry = std::make_shared<order::OrderBookEntry>(order, parent_limit);
                if (parent_limit->head_entry==nullptr){
                    parent_limit->head_entry = orderbook_entry;
                    parent_limit->tail_entry = orderbook_entry;
                }
                else{
                    std::shared_ptr<order::OrderBookEntry> tail_entry = parent_limit->tail_entry;
                    tail_entry->next_entry = orderbook_entry;
                    orderbook_entry->prev_entry = tail_entry;
                    parent_limit->tail_entry = orderbook_entry;
                }
                orderbook_.insert({order->order_id, orderbook_entry});
            }
        }
        
        bool RemoveOrder(std::shared_ptr<order::Order> order){
            const auto it = orderbook_.find(order->order_id);
            if(it != orderbook_.end()){
                std::shared_ptr<order::OrderBookEntry> cancel_orderbook_entry = it->second;
                //deal with orderbook_entry within the linklist of limit
                if ((cancel_orderbook_entry->prev_entry != nullptr) && (cancel_orderbook_entry->next_entry != nullptr)){
                    cancel_orderbook_entry->prev_entry->next_entry = cancel_orderbook_entry->next_entry;
                    cancel_orderbook_entry->next_entry->prev_entry = cancel_orderbook_entry->prev_entry;
                }
                else if (cancel_orderbook_entry->prev_entry != nullptr ){
                    cancel_orderbook_entry->prev_entry->next_entry = nullptr;
                }
                else if (cancel_orderbook_entry->next_entry != nullptr ){
                    cancel_orderbook_entry->next_entry->prev_entry = nullptr;
                }
                orderbook_.erase(order->order_id);
                
                //deal with limit level
                if ((cancel_orderbook_entry->parent_limit->head_entry==cancel_orderbook_entry) && (cancel_orderbook_entry->parent_limit->tail_entry==cancel_orderbook_entry)){
                    cancel_orderbook_entry->parent_limit->head_entry = nullptr;
                    cancel_orderbook_entry->parent_limit->tail_entry = nullptr;
                    // Remove limit from limit_levels_
                    std::vector<std::shared_ptr<order::Limit>> keep_limits_;
                    while(!limit_levels_.empty()){
                        if(limit_levels_.top()==cancel_orderbook_entry->parent_limit){
                            limit_levels_.pop();
                            break;
                        }
                        else{
                            keep_limits_.push_back(limit_levels_.top());
                            limit_levels_.pop();
                        }
                    }
                    for (int i=0; i<keep_limits_.size(); i++){
                        limit_levels_.push(keep_limits_[i]);
                    }
                    price_to_limit_map_.erase(order->price);
                }
                else if (cancel_orderbook_entry->parent_limit->head_entry==cancel_orderbook_entry){
                    cancel_orderbook_entry->parent_limit->head_entry = cancel_orderbook_entry->next_entry;
                }
                else{
                    cancel_orderbook_entry->parent_limit->tail_entry = cancel_orderbook_entry->prev_entry;
                }
               
                return true;
            }
            return false;
        }
        
        bool ContainsOrder(int64_t order_id) const{
//            return all_order_ids_.count(order_id)>0;
            return orderbook_.find(order_id) != orderbook_.end();
        }
        
        bool ContainsPrice(price::Price4 p) const{
            return price_to_limit_map_.find(p) != price_to_limit_map_.end();
        }
        std::shared_ptr<order::Limit> TopLimit(){
            if (! limit_levels_.empty()){
                return limit_levels_.top();
            }
            else{
                return nullptr;
            }
        }
        
        std::shared_ptr<order::Limit> GetLimit(price::Price4 p){
            return price_to_limit_map_[p];
        }
        
        std::shared_ptr<order::OrderBookEntry> GetOrderBookEntry(int64_t order_id){
            return orderbook_[order_id];
        }
        
    private:
        std::priority_queue<std::shared_ptr<order::Limit>, std::vector<std::shared_ptr<order::Limit>>, T> limit_levels_;
        std::unordered_map<price::Price4, std::shared_ptr<order::Limit>> price_to_limit_map_;
        std::unordered_map<int64_t, std::shared_ptr<order::OrderBookEntry>> orderbook_;
//        std::unordered_set<int64_t> all_order_ids_;
    };

    class BidOrderBook : public OrderBook<BidComparator>{
    public:
        BidOrderBook() = default;
        BidOrderBook(const OrderBook&) = delete;
        BidOrderBook(OrderBook&&) = delete;
        
        bool MatchOrder(std::shared_ptr<order::Order> input_order, std::shared_ptr<order::Order> target_order) const override{
            if (target_order == nullptr){
                return false;
            }
            else if (input_order->order_type==order::OrderType::MARKET){
                return true;
            }
            else{
                return input_order->price > target_order->price;
            }
        }
    };

    class AskOrderBook : public OrderBook<BidComparator>{
    public:
        AskOrderBook() = default;
        AskOrderBook(const OrderBook&) = delete;
        AskOrderBook(OrderBook&&) = delete;
        
        bool MatchOrder(std::shared_ptr<order::Order> input_order, std::shared_ptr<order::Order> target_order) const override{
            if (target_order == nullptr){
                return false;
            }
            else if (input_order->order_type==order::OrderType::MARKET){
                return true;
            }
            else{
                return input_order->price < target_order->price;
            }
        }
    };

}


#endif /* orderbook_hpp */
