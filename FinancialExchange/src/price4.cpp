//
//  price4.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/6/22.
//

#include <string>
#include "../include/price4.hpp"


namespace fe::price{
    constexpr int kScale4 = 10000;
    
    Price4::Price4(const std::string& str){
        unscaled_ = 0;
        int t = kScale4;
        bool exist_decimal_point = false;
        
        for (const char c : str) {
            if (c == '.'){
                exist_decimal_point = true;
                continue;
            }
            if (exist_decimal_point) {
                t /= 10;
            }
            if (t == 0){
                return;
            }
            unscaled_ = unscaled_ * 10 + (c - '0');
        }
        unscaled_ *= t;
    }

    std::string Price4::to_str() const{
        
        const std::string int_part = std::to_string(unscaled_/kScale4);
        long res = unscaled_ - unscaled_/kScale4*kScale4;
        std::string fraction = ".0000";
        
        int i = 4;
        int remove_zeros = 0;
        while (res%10==0 && i >= 0){
            res /= 10;
            --i;
            ++remove_zeros;
        }
        while(res > 0 && i >= 0){
            int d = res % 10;
            fraction[i] = d + '0';
            res /= 10;
            --i;
        }
        if (remove_zeros==4){
            return int_part;
        }
        else{
            return int_part + fraction.substr(0, 5-remove_zeros);
        }
    }
}


