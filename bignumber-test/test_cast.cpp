#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestCast : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestCast, test_types);

// TODO: test an explicit cast to the underlying type in BigInteger

TYPED_TEST(TestCast, CastToString) {
    BigInteger<TypeParam> zero = 0;
    EXPECT_EQ(std::string(zero), std::string("0"));

    BigInteger<TypeParam> eight = 8;
    EXPECT_EQ(std::string(eight), std::string("8"));

    BigInteger<TypeParam> negative = -10000;
    EXPECT_EQ(std::string(negative), std::string("-10000"));

    BigInteger<TypeParam> large = "1234567890123456789012345678901234567890123456789012345678901234567890";
    EXPECT_EQ(std::string(large), std::string("1234567890123456789012345678901234567890123456789012345678901234567890"));
}

TYPED_TEST(TestCast, CastToLongDouble) {
    const BigInteger<TypeParam> eight = 8;

    EXPECT_EQ(long double(eight), 8.0);

    EXPECT_EQ(long double(eight >> 3), 1.0);
    EXPECT_EQ(long double(eight >> 4), 0.0);

    EXPECT_EQ(long double(eight << 200), pow(2.0, 203));
    EXPECT_EQ(long double(eight << 2000), INFINITY);

    EXPECT_EQ(long double(-eight), -8.0);
}

TYPED_TEST(TestCast, CastToBoolean) {
    EXPECT_TRUE(bool(BigInteger<TypeParam>("-9223372036854775807")));
    EXPECT_TRUE(bool(BigInteger<TypeParam>(-1)));
    EXPECT_FALSE(bool(BigInteger<TypeParam>(0)));
    EXPECT_TRUE(bool(BigInteger<TypeParam>(1)));
    EXPECT_TRUE(bool(BigInteger<TypeParam>("18446744073709551616")));
    EXPECT_TRUE(bool(BigInteger<TypeParam>("100000000000000000000000000000")));
}
