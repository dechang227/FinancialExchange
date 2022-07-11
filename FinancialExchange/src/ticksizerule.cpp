//
//  ticksizerule.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 5/19/22.
//

#include <vector>

#include "nlohmann/json.hpp"
#include "../include/price4.hpp"
#include "../include/ticksizerule.hpp"

namespace fe::matchingengine{
    using nlohmann::json;
    using namespace fe::price;
    TickSizeRule::TickSizeRule(const json &data){
        ticks.reserve(data.size());
        for (const auto &input_tick : data.items()){
            Tick tick;
            for (const auto &item : input_tick.value().items()){
                if (item.key() == "from_price"){
                    tick.from_price = Price4(item.value().get<std::string>());
                }
                if (item.key() == "to_price"){
                    tick.to_price = Price4(item.value().get<std::string>());
                }
                if (item.key() == "tick_size"){
                    tick.tick_size = Price4(item.value().get<std::string>());
                }
            }
            ticks.push_back(tick);
        }
        
        std::sort(ticks.begin(), ticks.end(), [](const Tick &lhs, const Tick &rhs){return lhs.from_price < rhs.from_price;});
    }
    bool TickSizeRule::Validate() const{
        for (std::size_t i = 0; i < ticks.size() - 1; ++i){
            if (ticks[i].from_price >= ticks[i].to_price){
                return false;
            }
            if (ticks[i].to_price != ticks[i+1].from_price){
                return false;
            }
        }
        return ticks.empty() ? false : true;
    }
}
