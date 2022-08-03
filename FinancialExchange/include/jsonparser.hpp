//
//  jsonparser.h
//  FinancialExchange
//
//  Created by Dechang Chen on 4/15/22.
//

#ifndef jsonparser_h
#define jsonparser_h

#include <string>
#include <fstream>
#include "order.hpp"
#include "nlohmann/json.hpp"


namespace fe::order{
    using json = nlohmann::json;

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

    static void to_json(json& j, Order& o){
        j["time"] = o.time;
        j["order_id"] = o.order_id;
        j["symbol"] = o.symbol;
        j["price"] = o.price.to_str();
        j["quantity"] = o.quantity;
        
        switch (o.order_type){
            case OrderType::MARKET:
                j["order_type"] = "MARKET";
                break;
            case OrderType::LIMIT:
                j["order_type"] = "LIMIT";
                break;
            case OrderType::ICEBERG:
                j["order_type"] = "ICEBERG";
                break;
            default:
                j["order_type"] = "UNKNOWN";
                break;
        }
        
        switch (o.order_side){
            case OrderSide::BUY:
                j["order_side"] = "BUY";
                break;
            case OrderSide::SELL:
                j["order_side"] = "SELL";
                break;
            default:
                j["order_side"] = "UNKNOWN";
                break;
        }
        
        switch (o.order_status){
            case OrderStatus::NEW:
                j["order_status"] = "NEW";
                break;
            case OrderStatus::CANCEL:
                j["order_status"] = "CANCEL";
                break;
            default:
                j["order_status"] = "UNKNOWN";
                break;
        }
        
        switch (o.time_in_force){
            case TimeInForce::DAY:
                j["time_in_force"] = "DAY";
                break;
            case TimeInForce::IOC:
                j["time_in_force"] = "IOC";
                break;
            case TimeInForce::GTC:
                j["time_in_force"] = "TGC";
                break;
            default:
                j["time_in_force"] = "UNKNOWN";
                break;
        }
    }

    static std::vector<Order> ReadOrdersFromPath(const std::string &path){
        std::string line;
        std::ifstream infile(path);
        std::vector<Order> orders;
        while (std::getline(infile, line)){
            json j = json::parse(line);
            orders.emplace_back(j.get<Order>());
        }
        return orders;
    }
}


#endif /* jsonparser_h */
