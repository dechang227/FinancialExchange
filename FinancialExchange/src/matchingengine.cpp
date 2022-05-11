//
//  matchingengine.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/12/22.
//

#include <string>
#include "../include/order.hpp"
#include "../include/orderbook.hpp"
#include "../include/feedevent.hpp"
#include "../include/matchingengine.hpp"

namespace fe::matchingengine{
    
    namespace{
        using namespace fe::price;
        using namespace fe::order;
        using namespace fe::orderbook;
        using namespace fe::feedevent;
    
        template <class T>
        void UpdateQuantityAfterOrderCancelling(std::shared_ptr<Order> o, OrderBook<T>& ob, int64_t &pre_quantity, int64_t& post_quantity){
            std::shared_ptr<Limit> l = ob.GetLimit(o->price);
            pre_quantity = l->GetSize();
            ob.RemoveOrder(o);
            post_quantity = l->GetSize();
        }
    
        std::shared_ptr<ActionEvent> GetActionEvent(const Price4& p, const int64_t& pre_quantity, const int64_t post_quantity){
            if (pre_quantity == post_quantity){
                return nullptr;
            }
            if (post_quantity == 0){
                return std::make_shared<ActionEvent>("DELETE", p, post_quantity);
            }
            if (pre_quantity == 0){
                return std::make_shared<ActionEvent>("ADD", p, post_quantity);
            }
            return std::make_shared<ActionEvent>("MODIFY", p, post_quantity);
        }
        
        template <class T, class U>
        std::string ProcessInternal(std::shared_ptr<Order> new_order, std::unordered_map<int64_t, std::shared_ptr<Order>> & order_id_map, OrderBook<T> &orderbook_to_match, OrderBook<U> &orderbook_to_insert){
            FeedEvents feed_events{.symbol=new_order->symbol, .timestamp = new_order->time};
            std::unordered_map<Price4, int64_t> depth_update_prices_to_pre_quantities;
            const int64_t new_order_price_pre_quantity = (orderbook_to_insert.ContainsPrice(new_order->price)) ? orderbook_to_insert.GetLimit(new_order->price)->GetSize() : 0;
            int64_t new_order_price_post_quantity = 0;
            
            while(new_order->quantity != 0){
                std::shared_ptr<Limit> top_limit = orderbook_to_match.TopLimit();
                std::shared_ptr<Order> first_order = (top_limit==nullptr) ? nullptr : top_limit->head_entry->current_order;
                
                // If order can't be matched, add it to orderbook_to_insert
                if (!orderbook_to_match.MatchOrder(new_order, first_order)){
                    //market order should be canceled if there is no orders on orderbook
                    if (new_order->order_type == OrderType::MARKET && first_order == nullptr){
                        break;
                    }
                    //IOC order should be canceled immediately if not matched
                    if (new_order->time_in_force == TimeInForce::IOC){
                        break;
                    }
                    
                    order_id_map.insert({new_order->order_id, new_order});
                    orderbook_to_insert.InsertOrder(new_order);
                    new_order_price_post_quantity = orderbook_to_insert.GetLimit(new_order->price)->GetSize();
                    break;
                }
                
                depth_update_prices_to_pre_quantities.insert({top_limit->price, top_limit->GetSize()});
                
                const int64_t trade_quantity = std::min(new_order->quantity, first_order->quantity);
                if (new_order->quantity >= first_order->quantity){
                    new_order->quantity -= first_order->quantity;
                    orderbook_to_match.RemoveOrder(first_order);
                    order_id_map.erase(first_order->order_id);
                }
                else {
                    first_order->quantity -= new_order->quantity;
                    new_order->quantity = 0;
                }
                //update trade events
                feed_events.trade_events.push_back(TradeEvent{.price=top_limit->price, .quantity=trade_quantity});
            }
            //Update depth trade event
            //new order depth update if not matched
            std::shared_ptr<ActionEvent> new_order_action_event = GetActionEvent(new_order->price, new_order_price_pre_quantity, new_order_price_post_quantity);
            if (new_order_action_event != nullptr){
                if (new_order->order_side==OrderSide::BUY){
                    feed_events.depth_update_events.bid_events.push_back(new_order_action_event);
                }
                else if (new_order->order_side==OrderSide::SELL){
                    feed_events.depth_update_events.ask_events.push_back(new_order_action_event);
                }
            }
            //depth update for trades
            for (const auto &[price, pre_quantity]: depth_update_prices_to_pre_quantities){
                std::shared_ptr<ActionEvent> depth_update_action_event = GetActionEvent(price, pre_quantity, orderbook_to_match.GetLimit(new_order->price)->GetSize());
                if (new_order->order_side==OrderSide::BUY){
                    feed_events.depth_update_events.ask_events.push_back(depth_update_action_event);
                }
                else if (new_order->order_side==OrderSide::SELL){
                    feed_events.depth_update_events.bid_events.push_back(depth_update_action_event);
                }
            }
            return feed_events.to_str();
        }
    }
    
    MatchingEngine::MatchingEngine(){
    }
    
    std::string MatchingEngine::Process(std::shared_ptr<Order> order){
        std::string order_valid_status = ValidateOrder(order);
        if (order_valid_status!="Valid"){
            return order_valid_status;
        }
        if (order->order_status == OrderStatus::CANCEL){
            return Cancel(order);
        }
        if (order->order_side == OrderSide::BUY){
            return ProcessInternal(order, order_id_map_, ask_order_books_[order->symbol], bid_order_books_[order->symbol]);
        }
        if (order->order_side == OrderSide::SELL){
            return ProcessInternal(order, order_id_map_, bid_order_books_[order->symbol], ask_order_books_[order->symbol]);
        }
        return "This order can't be processed";
    }

    std::string MatchingEngine::ValidateOrder(std::shared_ptr<Order> order){
        if (order->time < 0){
            return "Invalid order timestamp";
        }
        if (order->order_id < 0){
            return "Invalid order id";
        }
        if (order->quantity <= 0) {
            return "Invalid order quantity";
        }
        return "Valid";
    }

    std::string MatchingEngine::Cancel(std::shared_ptr<Order> order){
        const auto kv = order_id_map_.find(order->order_id);
        if (kv == order_id_map_.end()){
            return "Order not found";
        }
        
        int64_t pre_quantity = 0;
        int64_t post_quantity = 0;
        if (order->order_side == OrderSide::BUY){
            UpdateQuantityAfterOrderCancelling(order, bid_order_books_[order->symbol], pre_quantity, post_quantity);
        }
        else {
            UpdateQuantityAfterOrderCancelling(order, ask_order_books_[order->symbol], pre_quantity, post_quantity);
        }
        
        std::shared_ptr<ActionEvent> action_event = GetActionEvent(order->price, pre_quantity, post_quantity);
        FeedEvents feed_events;
        feed_events.symbol = order->symbol;
        feed_events.timestamp = order->time;
        if (order->order_side == OrderSide::BUY){
            feed_events.depth_update_events.bid_events.push_back(action_event);
        }
        else {
            feed_events.depth_update_events.ask_events.push_back(action_event);
        }
        order_id_map_.erase(order->order_id);
        return feed_events.to_str();
    }
}
