//
//  jsonparser.h
//  FinancialExchange
//
//  Created by Dechang Chen on 4/15/22.
//

#ifndef jsonparser_h
#define jsonparser_h

#include <string>
#include "order.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace fe::order{

    template<class T>
    T GetValueInJson(const json& j, std::string key, const T& default_value){
        const auto itr = j.find(key);
        if(itr == j.end()){
            return default_value;
        }
        return itr->get<T>();
    }
    static void from_json(const json& j, Order& o){
        o.time = GetValueInJson<int>(j, "time", 0);
        o.order_id = GetValueInJson<int>(j, "order_id", 0);
        o.symbol = GetValueInJson<std::string>(j, "symbol", "");
        o.price = GetValueInJson<price::Price4>(j, "price", price::Price4("0"));
        o.quantity = GetValueInJson<int>(j, "quantity", 0);
        
        const std::string order_type = GetValueInJson<std::string>(j, "order_type", "");
        if (order_type == "MARKET"){
            o.order_type = OrderType::MARKET;
        }
        else if (order_type == "LIMIT"){
            o.order_type = OrderType::LIMIT;
        }
        else if (order_type == "ICEBERG"){
            o.order_type = OrderType::ICEBERG;
        }
        else{
            o.order_type = OrderType::UNKNOWN;
        }
        
        const std::string order_side = GetValueInJson<std::string>(j, "order_side", "");
        if (order_side == "BUY"){
            o.order_side = OrderSide::BUY;
        }
        else if (order_side == "SELL"){
            o.order_side = OrderSide::SELL;
        }
        else{
            o.order_side = OrderSide::UNKNOWN;
        }

        const std::string order_status = GetValueInJson<std::string>(j, "order_status", "");
        if (order_status == "NEW"){
            o.order_status = OrderStatus::NEW;
        }
        else if (order_status == "CANCEL"){
            o.order_status = OrderStatus::CANCEL;
        }
        else{
            o.order_status = OrderStatus::UNKNOWN;
        }
        
        const std::string time_in_force = GetValueInJson<std::string>(j, "time_in_force", "");
        if (time_in_force == "DAY"){
            o.time_in_force = TimeInForce::DAY;
        }
        else if (time_in_force == "IOC"){
            o.time_in_force = TimeInForce::IOC;
        }
        else if (time_in_force == "GTC") {
            o.time_in_force = TimeInForce::GTC;
        }
        else{
            o.time_in_force = TimeInForce::UNKNOWN;
        }
    }

//    static void to_json(json& j, Order& o){
//        j = json({
//                {"time", o.time},
//                {"order_id", o.order_id},
//                {"symbol", o.symbol},
//                {"price", o.price.to_str()},
//                {"quantity", o.quantity},
//                {"order_type", "LIMIT"},
//                {"order_side", "BUY"},
//                {"order_status", "NEW"},
//                {"time_in_force", "GTC"}
//                });
//    }
}


#endif /* jsonparser_h */
