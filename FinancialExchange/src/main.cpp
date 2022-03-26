//
//  main.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/6/22.
//

#include <iostream>
#include "../include/price4.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    fe::Price4 p4_sample("1230");
    
    std::cout << "Hello, World!\n" << p4_sample.unscaled() << "\n";
    return 0;
}
