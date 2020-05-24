#include "gtest/gtest.h"

#include "bignumber.hpp"

// TODO: test an explicit cast to the underlying type in BigInteger

TEST(TestCast, CastToString) {
    BigInteger zero = 0;
    EXPECT_EQ(std::string(zero), std::string("0"));

    BigInteger eight = 8;
    EXPECT_EQ(std::string(eight), std::string("8"));

    BigInteger negative = -10000;
    EXPECT_EQ(std::string(negative), std::string("-10000"));

    BigInteger large = "1234567890123456789012345678901234567890123456789012345678901234567890";
    EXPECT_EQ(std::string(large), std::string("1234567890123456789012345678901234567890123456789012345678901234567890"));
}

TEST(TestCast, CastToLongDouble) {
    const BigInteger eight = 8;

    EXPECT_EQ(long double(eight), 8.0);

    EXPECT_EQ(long double(eight >> 3), 1.0);
    EXPECT_EQ(long double(eight >> 4), 0.0);

    EXPECT_EQ(long double(eight << 200), pow(2.0, 203));
    EXPECT_EQ(long double(eight << 2000), INFINITY);

    EXPECT_EQ(long double(-eight), -8.0);
}

TEST(TestCast, CastToBoolean) {
    EXPECT_TRUE(bool(BigInteger("-9223372036854775807")));
    EXPECT_TRUE(bool(BigInteger(-1)));
    EXPECT_FALSE(bool(BigInteger(0)));
    EXPECT_TRUE(bool(BigInteger(1)));
    EXPECT_TRUE(bool(BigInteger("18446744073709551616")));
    EXPECT_TRUE(bool(BigInteger("100000000000000000000000000000")));
}
