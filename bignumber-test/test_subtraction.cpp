#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestSubtraction : public testing::Test { typedef T Digit; };
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestSubtraction, test_types);

TYPED_TEST(TestSubtraction, SubtractsBigIntegers) {
    const BigInteger<TypeParam> two = 2;
    const BigInteger<TypeParam> one = 1;
    EXPECT_EQ(two - two, 0);
    EXPECT_EQ(two - one, 1);
}

TYPED_TEST(TestSubtraction, SubtractsBigIntegersAndPrimitiveIntegers) {
    const BigInteger<TypeParam> two = 2;
    EXPECT_EQ(two - 1, 1);
    EXPECT_EQ(two - 2, 0);
    EXPECT_EQ(2 - two, 0);
    EXPECT_EQ(4 - two, 2);
}

TYPED_TEST(TestSubtraction, SubtractsBigIntegersAndDigits) {
    EXPECT_EQ(BigInteger<TypeParam>(2) - Digit(0), 2);
    EXPECT_EQ(BigInteger<TypeParam>(2) - Digit(1), 1);
    EXPECT_EQ(BigInteger<TypeParam>(2) - Digit(2), 0);
    EXPECT_EQ(BigInteger<TypeParam>(0) - Digit(1), -1);
    EXPECT_EQ(BigInteger<TypeParam>("100000000000000000000000000000000000") - Digit(1), "99999999999999999999999999999999999");
}

TYPED_TEST(TestSubtraction, SubtractsBigIntegersAndStrings) {
    const BigInteger<TypeParam> two = 2;
    EXPECT_EQ(two - "1", "1");
    EXPECT_EQ(two - "2", "0");
    EXPECT_EQ("2" - two, "0");
    EXPECT_EQ("4" - two, "2");
}

TYPED_TEST(TestSubtraction, SubtractsVeryBigIntegers) {
    const BigInteger<TypeParam> very_long_number = "1000000000000000000000";
    const BigInteger<TypeParam> one = 1;
    EXPECT_EQ(very_long_number - very_long_number, 0);
    EXPECT_EQ(very_long_number - one, "999999999999999999999");
}

TYPED_TEST(TestSubtraction, SubtractsAndAssignsBigIntegers) {
    BigInteger<TypeParam> number = 2;
    BigInteger<TypeParam> one = 1;
    EXPECT_EQ(number -= one, 1);
    EXPECT_EQ(number -= number, 0);
    EXPECT_EQ(number, 0);
}

TYPED_TEST(TestSubtraction, SubtractsAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger<TypeParam> number = 2;
    EXPECT_EQ(number -= 1, 1);
    EXPECT_EQ(number -= 1, 0);
    EXPECT_EQ(number, 0);
}

TYPED_TEST(TestSubtraction, SubtractsAndAssignsBigIntegersAndStrings) {
    BigInteger<TypeParam> number = 2;
    EXPECT_EQ(number -= "1", 1);
    EXPECT_EQ(number -= "1", 0);
    EXPECT_EQ(number, 0);
}

TYPED_TEST(TestSubtraction, SubtractsAndAssignsVeryBigIntegers) {
    BigInteger<TypeParam> very_long_number = "100000000000000000000000";
    EXPECT_EQ(very_long_number -= "10000000000000000000000", "90000000000000000000000");
    EXPECT_EQ(very_long_number -= "1000000000000000000000", "89000000000000000000000");
    EXPECT_EQ(very_long_number -= very_long_number, 0);
    EXPECT_EQ(very_long_number, 0);
}
