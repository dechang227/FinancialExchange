//
//  order.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/12/22.
//

#include "../include/order.hpp"

namespace fe::order{

    bool Limit::IsEmpty() const{
        return head_entry==nullptr && tail_entry==nullptr;
    }

    OrderSide Limit::GetOrderSide() const{
        if(this->IsEmpty()){
            return OrderSide::UNKNOWN;
        }
        else{
            return head_entry->current_order->order_side;
        }
    }
    
    int Limit::GetOrderCounts() const{
        int order_count = 0;
        std::shared_ptr<OrderBookEntry> current_entry = head_entry;
        while(current_entry != nullptr){
            if(current_entry->current_order->quantity != 0){
                order_count += 1;
            }
            current_entry = current_entry->next_entry;
        }
        return order_count;
    }

    int64_t Limit::GetSize() const{
        int64_t size = 0;
        std::shared_ptr<OrderBookEntry> current_entry = head_entry;
        while(current_entry != nullptr){
            size += current_entry->current_order->quantity;
            current_entry = current_entry->next_entry;
        }
        return size;
    }
}
