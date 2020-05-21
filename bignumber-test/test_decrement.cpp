#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestDecrement, DecrementsSmallNumbers) {
    EXPECT_EQ(--BigInteger(-1), -2);
    EXPECT_EQ(--BigInteger(0), -1);
    EXPECT_EQ(--BigInteger(1), 0);
    EXPECT_EQ(--BigInteger(-1000), -1001);
    EXPECT_EQ(--BigInteger(1000), 999);
    EXPECT_EQ(--BigInteger((__int32)-2147483648), "-2147483649");
    EXPECT_EQ(--BigInteger((unsigned __int32)~0u), "4294967294");
}

TEST(TestDecrement, PrefixDecrement) {
    BigInteger number = 1;
    EXPECT_EQ(--number, 0);
    EXPECT_EQ(number, 0);

    BigInteger long_number = "10000000000000000000000000000000000000000000";
    EXPECT_EQ(--long_number, "9999999999999999999999999999999999999999999");
    EXPECT_EQ(long_number, "9999999999999999999999999999999999999999999");
}

TEST(TestDecrement, PostfixDecrement) {
    BigInteger number = 1;
    EXPECT_EQ(number--, 1);
    EXPECT_EQ(number, 0);

    BigInteger long_number = "10000000000000000000000000000000000000000000";
    EXPECT_EQ(long_number--, "10000000000000000000000000000000000000000000");
    EXPECT_EQ(long_number, "9999999999999999999999999999999999999999999");
}

TEST(TestDecrement, DecrementsLargeNegativeNumbers) {
    BigInteger number = "-1000000000000000000000000000000000000000000000000000000";
    EXPECT_EQ(--number, "-1000000000000000000000000000000000000000000000000000001");
}
