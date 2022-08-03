//
//  datapublisher.hpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/12/22.
//

#ifndef datapublisher_hpp
#define datapublisher_hpp

#include <stdio.h>
//#include "glog/logging.h"
#include "feedevent.hpp"

namespace fe::publisher{
using namespace fe::feedevent;
    class DataPublisher{
    public:
        DataPublisher(){}
        DataPublisher(const DataPublisher &) = delete;
        DataPublisher(DataPublisher &&) = delete;
        ~DataPublisher() = default;
        
        void Publish(const std::string &message) const{
            return;
        };
    };
}


#endif /* datapublisher_hpp */
