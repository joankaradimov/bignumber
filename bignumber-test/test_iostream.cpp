#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestIOStream, ReadsNumbers) {
    BigInteger number;
    std::stringstream str;    
    str << "1000 0 -1 25";

    str >> number;
    EXPECT_EQ(number, 1000);

    str >> number;
    EXPECT_EQ(number, 0);

    str >> number;
    EXPECT_EQ(number, -1);

    str >> number;
    EXPECT_EQ(number, 25);
}

TEST(TestIOStream, ReadsBigNumbers) {
    BigInteger number;
    std::stringstream str;
    str << "1000000000000000000000000000000000000000000000 -1000000000000000000000000000000000000000000000";

    str >> number;
    EXPECT_EQ(number, "1000000000000000000000000000000000000000000000");

    str >> number;
    EXPECT_EQ(number, "-1000000000000000000000000000000000000000000000");
}

TEST(TestIOStream, WritesNumbers) {
    std::stringstream str;
    BigInteger eight = 8;
    BigInteger minus_one = -1;
    BigInteger nonillion = "1000000000000000000000000000000";
    BigInteger zero = 0;

    str << eight << ';' << minus_one << ';' << nonillion << ';' << zero;

    EXPECT_EQ(str.str(), "8;-1;1000000000000000000000000000000;0");
}
