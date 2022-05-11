//
//  price4.hpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/6/22.
//

#ifndef price4_hpp
#define price4_hpp

#include <stdio.h>
#include <string>

namespace fe::price{
    class Price4 {
    public:
        Price4() = default;
        explicit Price4(long unscaled) : unscaled_(unscaled) {}
        // convert from string
//        explicit
        Price4(const std::string& str);
        
        Price4(const Price4& p):unscaled_(p.unscaled_){};
        
        long unscaled() const { return unscaled_; }
        // convert to string
        std::string to_str() const;
        
        bool operator==(const Price4 &that) const{
            return this->unscaled() == that.unscaled();
        }
        bool operator!=(const Price4 &that) const{
            return this->unscaled() != that.unscaled();
        }
        bool operator>=(const Price4 &that) const{
            return this->unscaled() >= that.unscaled();
        }
        bool operator>(const Price4 &that) const{
            return this->unscaled() > that.unscaled();
        }
        bool operator<=(const Price4 &that) const{
            return this->unscaled() <= that.unscaled();
        }
        bool operator<(const Price4 &that) const{
            return this->unscaled() < that.unscaled();
        }
        
    private:
        long unscaled_;
    };
}


namespace std
{
  template <>
  struct hash<fe::price::Price4>
  {
    std::size_t operator()(const fe::price::Price4 &k) const
    {
      return std::hash<long>()(k.unscaled());
    }
  };
}

#endif /* price4_hpp */

