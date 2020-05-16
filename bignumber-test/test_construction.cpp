#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestConstruction, ConstructsFromIntegers) {
    BigInteger zero = 0;
    EXPECT_EQ(zero, 0);

    BigInteger one = 1;
    EXPECT_EQ(one, 1);
}

TEST(TestConstruction, ConstructsFromStrings) {
    BigInteger hundred = "100";
    EXPECT_EQ(hundred, 100);

    BigInteger billion = "1000000000";
    EXPECT_EQ(billion, 1000000000);

    BigInteger c = "299792458";
    EXPECT_EQ(c, 299792458);
}

TEST(TestConstruction, ConstructsFromOtherBigIntegers) {
    BigInteger one = 1;
    BigInteger another_one = one;
    EXPECT_EQ(one, another_one);
}