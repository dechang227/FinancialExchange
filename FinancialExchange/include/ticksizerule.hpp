//
//  ticksizerule.hpp
//  FinancialExchange
//
//  Created by Dechang Chen on 5/19/22.
//

#ifndef ticksizerule_hpp
#define ticksizerule_hpp

#include <stdio.h>

#include <vector>

#include "nlohmann/json.hpp"
#include "../include/price4.hpp"

namespace fe::matchingengine{
    using nlohmann::json;
    struct Tick{
        fe::price::Price4 from_price;
        fe::price::Price4 to_price;
        fe::price::Price4 tick_size;
    };

    class TickSizeRule{
        public:
            TickSizeRule() = default;
            TickSizeRule(const json &data);
            bool Validate() const;
            const std::vector<Tick> &GetTicks() const{
                return ticks;
            }
        private:
            std::vector<Tick> ticks;
    };
}


#endif /* ticksizerule_hpp */
