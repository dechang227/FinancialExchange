//
//  price4_unittest.cpp
//  FinancialExchange
//
//  Created by Dechang Chen on 3/19/22.
//

#include <stdio.h>
#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../FinancialExchange/include/price4.hpp"


namespace fe{
    namespace {
        TEST(Price4Test, FromString){
            const Price4 p1("1234");
            EXPECT_EQ(12340000, p1.unscaled());
            const Price4 p2("123.4");
            EXPECT_EQ(1234000, p2.unscaled());
            const Price4 p3("12.34");
            EXPECT_EQ(123400, p3.unscaled());
            const Price4 p4("1.234");
            EXPECT_EQ(12340, p4.unscaled());
            const Price4 p5("0.1234");
            EXPECT_EQ(1234, p5.unscaled());
            const Price4 p6("0.01234");
            EXPECT_EQ(123, p6.unscaled());
            const Price4 p7("0.001234");
            EXPECT_EQ(12, p7.unscaled());
        }

        TEST(Price4Test, ToString){
            const Price4 p1(0.1234);
            EXPECT_EQ("0", p1.to_str());
            const Price4 p2(1.234);
            EXPECT_EQ("0.0001", p2.to_str());
            const Price4 p3(12.34);
            EXPECT_EQ("0.0012", p3.to_str());
            const Price4 p4(123.4);
            EXPECT_EQ("0.0123", p4.to_str());
            const Price4 p5(1234);
            EXPECT_EQ("0.1234", p5.to_str());
            const Price4 p6(12345);
            EXPECT_EQ("1.2345", p6.to_str());
            const Price4 p7(123456);
            EXPECT_EQ("12.3456", p7.to_str());
            const Price4 p8(1234000);
            EXPECT_EQ("123.4", p8.to_str());
            const Price4 p9(1234001);
            EXPECT_EQ("123.4001", p9.to_str());
        }
    
        TEST(Price4Test, TestOperators){
            const Price4 p1(1);
            const Price4 p2(2);
            EXPECT_FALSE(p1==p2);
            EXPECT_TRUE(p1!=p2);
            EXPECT_FALSE(p1>=p2);
            EXPECT_TRUE(p1>=p1);
            EXPECT_FALSE(p1>p2);
            EXPECT_TRUE(p1<=p2);
            EXPECT_TRUE(p2<=p2);
            EXPECT_TRUE(p1<p2);
        }
    }
}
