//
//  order.hpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/12/22.
//

#ifndef order_hpp
#define order_hpp

#include <stdio.h>
#include <string>
//#include <ctime>
#include "price4.hpp"
//#include "limit.hpp"
//using namespace std;

namespace fe::order{
    enum class OrderType{
        UNKNOWN,
        MARKET,
        LIMIT,
        ICEBERG
    };

    enum class OrderStatus{
        UNKNOWN,
        NEW,
        CANCEL
    };

    enum class OrderSide{
        UNKNOWN,
        BUY,
        SELL
    };

    enum class TimeInForce{
        UNKNOWN,
        DAY,
        IOC,
        GTC
    };

    class OrderBookEntry;

    struct Limit{
        Limit(const price::Price4& price):price(price){};
        price::Price4 price;
        std::shared_ptr<OrderBookEntry> head_entry;
        std::shared_ptr<OrderBookEntry> tail_entry;
        bool IsEmpty() const;
        OrderSide GetOrderSide() const;
        int GetOrderCounts() const;
        int64_t GetSize() const;
    };

    struct Order{
        int64_t time;
        int64_t order_id;
        std::string symbol;
        price::Price4 price;
        int64_t quantity;
        OrderType order_type;
        OrderSide order_side;
        OrderStatus order_status;
        TimeInForce time_in_force;
    };

    struct OrderBookEntry{
        OrderBookEntry(const std::shared_ptr<Order>& current_order, const std::shared_ptr<Limit>& parent_limit): current_order(current_order),parent_limit(parent_limit){
        };
        std::shared_ptr<Order> current_order;
        std::shared_ptr<Limit> parent_limit;
        std::shared_ptr<OrderBookEntry> prev_entry;
        std::shared_ptr<OrderBookEntry> next_entry;
        
//        int64_t getEntryTime() const{return currentOrder.time;};
//        Order getCurrentOrder() const {return currentOrder;};
//        std::shared_ptr<Limit> getParentLimit() const {return parentLimit;};
    };
}
#endif /* order_hpp */
