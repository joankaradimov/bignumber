#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestMultiplication : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestMultiplication, test_types);

TYPED_TEST(TestMultiplication, MultipliesBigIntegers) {
    const BigInteger<TypeParam> two = 2;
    EXPECT_EQ(two * two, 4);
}

TYPED_TEST(TestMultiplication, MultipliesBigIntegersAndPrimitiveIntegers) {
    const BigInteger<TypeParam> two = 2;
    EXPECT_EQ(two * 2, 4);
    EXPECT_EQ(2 * two, 4);

    EXPECT_EQ(two * 5, 10);
    EXPECT_EQ(5 * two, 10);
}

TYPED_TEST(TestMultiplication, MultipliesBigIntegersAndStrings) {
    const BigInteger<TypeParam> two = 2;
    EXPECT_EQ(two * "2", 4);
    EXPECT_EQ("2" * two, 4);

    EXPECT_EQ(two * "5", 10);
    EXPECT_EQ("5" * two, 10);
}

TYPED_TEST(TestMultiplication, MultipliesVeryBigIntegers) {
    const BigInteger<TypeParam> very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number * very_long_number, "1000000000000000000000000000000000000000000");
    EXPECT_EQ(very_long_number * "500000000000000000000000000", "500000000000000000000000000000000000000000000000");
    EXPECT_EQ("600000000000000000000000000" * very_long_number, "600000000000000000000000000000000000000000000000");
}

TYPED_TEST(TestMultiplication, MultipliesAndAssignsBigIntegers) {
    BigInteger<TypeParam> number = 2;
    EXPECT_EQ(number *= number, 4);
    EXPECT_EQ(number, 4);
}

TYPED_TEST(TestMultiplication, MultipliesAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger<TypeParam> number = 2;
    EXPECT_EQ(number *= 2, 4);
    EXPECT_EQ(number, 4);
}

TYPED_TEST(TestMultiplication, MultipliesAndAssignsBigIntegersAndStrings) {
    BigInteger<TypeParam> number = 2;
    EXPECT_EQ(number *= "2", 4);
    EXPECT_EQ(number, 4);
}

TYPED_TEST(TestMultiplication, MultipliesAndAssignsVeryBigIntegers) {
    BigInteger<TypeParam> very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number *= very_long_number, "1000000000000000000000000000000000000000000");
    EXPECT_EQ(very_long_number, "1000000000000000000000000000000000000000000");
}
