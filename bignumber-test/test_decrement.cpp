#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestDecrement : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestDecrement, test_types);

TYPED_TEST(TestDecrement, DecrementsSmallNumbers) {
    EXPECT_EQ(--BigInteger<TypeParam>(-1), -2);
    EXPECT_EQ(--BigInteger<TypeParam>(0), -1);
    EXPECT_EQ(--BigInteger<TypeParam>(1), 0);
    EXPECT_EQ(--BigInteger<TypeParam>(-1000), -1001);
    EXPECT_EQ(--BigInteger<TypeParam>(1000), 999);
    EXPECT_EQ(--BigInteger<TypeParam>((__int32)-2147483648), "-2147483649");
    EXPECT_EQ(--BigInteger<TypeParam>((unsigned __int32)~0u), "4294967294");
}

TYPED_TEST(TestDecrement, PrefixDecrement) {
    BigInteger<TypeParam> number = 1;
    EXPECT_EQ(--number, 0);
    EXPECT_EQ(number, 0);

    BigInteger<TypeParam> long_number = "10000000000000000000000000000000000000000000";
    EXPECT_EQ(--long_number, "9999999999999999999999999999999999999999999");
    EXPECT_EQ(long_number, "9999999999999999999999999999999999999999999");
}

TYPED_TEST(TestDecrement, PostfixDecrement) {
    BigInteger<TypeParam> number = 1;
    EXPECT_EQ(number--, 1);
    EXPECT_EQ(number, 0);

    BigInteger<TypeParam> long_number = "10000000000000000000000000000000000000000000";
    EXPECT_EQ(long_number--, "10000000000000000000000000000000000000000000");
    EXPECT_EQ(long_number, "9999999999999999999999999999999999999999999");
}

TYPED_TEST(TestDecrement, DecrementsLargeNegativeNumbers) {
    BigInteger<TypeParam> number = "-1000000000000000000000000000000000000000000000000000000";
    EXPECT_EQ(--number, "-1000000000000000000000000000000000000000000000000000001");
}
