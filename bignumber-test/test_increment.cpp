#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestIncrement : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestIncrement, test_types);

TYPED_TEST(TestIncrement, IncrementsSmallNumbers) {
    EXPECT_EQ(++BigInteger<TypeParam>(-1), 0);
    EXPECT_EQ(++BigInteger<TypeParam>(0), 1);
    EXPECT_EQ(++BigInteger<TypeParam>(1), 2);
    EXPECT_EQ(++BigInteger<TypeParam>(-1000), -999);
    EXPECT_EQ(++BigInteger<TypeParam>(1000), 1001);
    EXPECT_EQ(++BigInteger<TypeParam>((__int32)-2147483648), -2147483647);
    EXPECT_EQ(++BigInteger<TypeParam>((unsigned __int32) ~0u), "4294967296");
}

TYPED_TEST(TestIncrement, PrefixIncrement) {
    BigInteger<TypeParam> number = 1;
    EXPECT_EQ(++number, 2);
    EXPECT_EQ(number, 2);

    BigInteger<TypeParam> long_number = "10000000000000000000000000000000000000000000";
    EXPECT_EQ(++long_number, "10000000000000000000000000000000000000000001");
    EXPECT_EQ(long_number, "10000000000000000000000000000000000000000001");
}

TYPED_TEST(TestIncrement, PostfixIncrement) {
    BigInteger<TypeParam> number = 1;
    EXPECT_EQ(number++, 1);
    EXPECT_EQ(number, 2);

    BigInteger<TypeParam> long_number = "10000000000000000000000000000000000000000000";
    EXPECT_EQ(long_number++, "10000000000000000000000000000000000000000000");
    EXPECT_EQ(long_number, "10000000000000000000000000000000000000000001");
}

TYPED_TEST(TestIncrement, IncrementsLargeNegativeNumbers) {
    BigInteger<TypeParam> number = "-1000000000000000000000000000000000000000000000000000000";
    EXPECT_EQ(++number, "-999999999999999999999999999999999999999999999999999999");
}
