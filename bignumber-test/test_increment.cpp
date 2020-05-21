#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestIncrement, IncrementsSmallNumbers) {
    EXPECT_EQ(++BigInteger(-1), 0);
    EXPECT_EQ(++BigInteger(0), 1);
    EXPECT_EQ(++BigInteger(1), 2);
    EXPECT_EQ(++BigInteger(-1000), -999);
    EXPECT_EQ(++BigInteger(1000), 1001);
    EXPECT_EQ(++BigInteger((__int32)-2147483648), -2147483647);
    EXPECT_EQ(++BigInteger((unsigned __int32) ~0u), "4294967296");
}

TEST(TestIncrement, PrefixIncrement) {
    BigInteger number = 1;
    EXPECT_EQ(++number, 2);
    EXPECT_EQ(number, 2);

    BigInteger long_number = "10000000000000000000000000000000000000000000";
    EXPECT_EQ(++long_number, "10000000000000000000000000000000000000000001");
    EXPECT_EQ(long_number, "10000000000000000000000000000000000000000001");
}

TEST(TestIncrement, PostfixIncrement) {
    BigInteger number = 1;
    EXPECT_EQ(number++, 1);
    EXPECT_EQ(number, 2);

    BigInteger long_number = "10000000000000000000000000000000000000000000";
    EXPECT_EQ(long_number++, "10000000000000000000000000000000000000000000");
    EXPECT_EQ(long_number, "10000000000000000000000000000000000000000001");
}

TEST(TestIncrement, IncrementsLargeNegativeNumbers) {
    BigInteger number = "-1000000000000000000000000000000000000000000000000000000";
    EXPECT_EQ(++number, "-999999999999999999999999999999999999999999999999999999");
}
