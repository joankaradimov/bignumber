#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestAddition : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestAddition, test_types);

TYPED_TEST(TestAddition, AddsBigIntegers) {
    const BigInteger<TypeParam> two = 2;
    EXPECT_EQ(two + two, 4);
}

TYPED_TEST(TestAddition, AddsBigIntegersAndPrimitiveIntegers) {
    const BigInteger<TypeParam> two = 2;
    EXPECT_EQ(two + 2, 4);
    EXPECT_EQ(2 + two, 4);

    EXPECT_EQ(two + 8, 10);
    EXPECT_EQ(8 + two, 10);
}

TYPED_TEST(TestAddition, AddsBigIntegersAndDigits) {
    EXPECT_EQ(BigInteger<TypeParam>(2) + TypeParam(0), 2);
    EXPECT_EQ(TypeParam(0) + BigInteger<TypeParam>(2), 2);

    EXPECT_EQ(BigInteger<TypeParam>(2) + TypeParam(2), 4);
    EXPECT_EQ(TypeParam(2) + BigInteger<TypeParam>(2), 4);

    EXPECT_EQ(BigInteger<TypeParam>(2) + TypeParam(8), 10);
    EXPECT_EQ(TypeParam(8) + BigInteger<TypeParam>(2), 10);

    EXPECT_EQ(BigInteger<TypeParam>(-1) + TypeParam(1), 0);
    EXPECT_EQ(BigInteger<TypeParam>("100000000000000000000000000000000000") + TypeParam(1), "100000000000000000000000000000000001");
}

TYPED_TEST(TestAddition, AddsBigIntegersAndStrings) {
    const BigInteger<TypeParam> two = 2;
    EXPECT_EQ(two + "2", 4);
    EXPECT_EQ("2" + two, 4);

    EXPECT_EQ(two + "8", 10);
    EXPECT_EQ("8" + two, 10);
}

TYPED_TEST(TestAddition, AddsVeryBigIntegers) {
    const BigInteger<TypeParam> very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number + very_long_number, "2000000000000000000000");

    EXPECT_EQ(very_long_number + "500000000000000000000000000000000000000000000000000000", "500000000000000000000000000000001000000000000000000000");
    EXPECT_EQ("600000000000000000000000000000000000000000000000000000" + very_long_number, "600000000000000000000000000000001000000000000000000000");
}

TYPED_TEST(TestAddition, AddsAndAssignsBigIntegers) {
    BigInteger<TypeParam> number = 2;
    EXPECT_EQ(number += number, 4);
    EXPECT_EQ(number, 4);
}

TYPED_TEST(TestAddition, AddsAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger<TypeParam> number = 2;
    EXPECT_EQ(number += 2, 4);
    EXPECT_EQ(number, 4);
}

TYPED_TEST(TestAddition, AddsAndAssignsBigIntegersAndStrings) {
    BigInteger<TypeParam> number = 2;
    EXPECT_EQ(number += "2", 4);
    EXPECT_EQ(number, 4);
}

TYPED_TEST(TestAddition, AddsAndAssignsVeryBigIntegers) {
    BigInteger<TypeParam> very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number += very_long_number, "2000000000000000000000");
    EXPECT_EQ(very_long_number, "2000000000000000000000");
}
