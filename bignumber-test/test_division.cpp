#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestDivision : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestDivision, test_types);

TYPED_TEST(TestDivision, DividesBigIntegers) {
    const BigInteger<TypeParam> four = 4;
    const BigInteger<TypeParam> two = 2;
    EXPECT_EQ(four / four, 1);
    EXPECT_EQ(four / two, 2);
}

TYPED_TEST(TestDivision, DividesBigIntegersAndPrimitiveIntegers) {
    const BigInteger<TypeParam> four = 4;
    EXPECT_EQ(four / 2, 2);
    EXPECT_EQ(four / 1, 4);
    EXPECT_EQ(8 / four, 2);
    EXPECT_EQ(400 / four, 100);
    EXPECT_EQ(BigInteger<TypeParam>("400000000000000000000000000000000000000") / 4, "100000000000000000000000000000000000000");
}

TYPED_TEST(TestDivision, DividesBigIntegersAndStrings) {
    const BigInteger<TypeParam> four = 4;
    EXPECT_EQ(four / "2", "2");
    EXPECT_EQ(four / "1", "4");
    EXPECT_EQ("8" / four, "2");
    EXPECT_EQ("400" / four, 100);
}

TYPED_TEST(TestDivision, DividesVeryBigIntegers) {
    const BigInteger<TypeParam> very_long_number = "1000000000000000000000000000000000000000000";
    const BigInteger<TypeParam> another_long_number = "1000000000000000000000000";
    EXPECT_EQ(very_long_number / another_long_number, "1000000000000000000");
}

TYPED_TEST(TestDivision, DividesAndRoundsDown) {
    const BigInteger<TypeParam> very_long_number = "1000000000000000000000000000000000000000000";
    EXPECT_EQ(very_long_number / "33333333333333333333333333333", "30000000000000");
    EXPECT_EQ(very_long_number / 3, "333333333333333333333333333333333333333333");

    EXPECT_EQ(BigInteger<TypeParam>(1) / 2, 0);
}

TYPED_TEST(TestDivision, DividesAndAssignsBigIntegers) {
    BigInteger<TypeParam> number = 4;
    BigInteger<TypeParam> two = 2;
    EXPECT_EQ(number /= two, 2);
    EXPECT_EQ(number /= two, 1);
    EXPECT_EQ(number, 1);
}

TYPED_TEST(TestDivision, DividesAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger<TypeParam> number = 4;
    EXPECT_EQ(number /= 2, 2);
    EXPECT_EQ(number /= 2, 1);
    EXPECT_EQ(number, 1);
}

TYPED_TEST(TestDivision, DividesAndAssignsBigIntegersAndStrings) {
    BigInteger<TypeParam> number = 4;
    EXPECT_EQ(number /= "2", 2);
    EXPECT_EQ(number /= "2", 1);
    EXPECT_EQ(number, 1);
}

TYPED_TEST(TestDivision, DividesAndAssignsVeryBigIntegers) {
    BigInteger<TypeParam> very_long_number = "1000000000000000000000000000000000000000000000000000";
    EXPECT_EQ(very_long_number /= "1000000000000000000000", "1000000000000000000000000000000");
    EXPECT_EQ(very_long_number /= very_long_number, 1);
    EXPECT_EQ(very_long_number, 1);
}

TYPED_TEST(TestDivision, DivisionByZero) {
    BigInteger<TypeParam> number = 1;
    EXPECT_DEATH(number / 0, "");
}
