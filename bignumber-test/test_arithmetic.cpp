#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestArithmetic, AddsBigIntegers) {
    const BigInteger two = 2;
    EXPECT_EQ(two + two, 4);
}

TEST(TestArithmetic, AddsVeryBigIntegers) {
    const BigInteger very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number + very_long_number, "2000000000000000000000");
}

TEST(TestArithmetic, AddsBigIntegersAndPrimitiveIntegers) {
    const BigInteger two = 2;
    EXPECT_EQ(two + 2, 4);
    EXPECT_EQ(2 + two, 4);
}

TEST(TestArithmetic, AddsBigIntegersAndStrings) {
    const BigInteger two = 2;
    EXPECT_EQ(two + "2", 4);
    EXPECT_EQ("2" + two, 4);
}

TEST(TestArithmetic, AddsAndAssignsBigIntegers) {
    BigInteger number = 2;
    EXPECT_EQ(number += number, 4);
    EXPECT_EQ(number, 4);
}

TEST(TestArithmetic, AddsAndAssignsVeryBigIntegers) {
    BigInteger very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number += very_long_number, "2000000000000000000000");
    EXPECT_EQ(very_long_number, "2000000000000000000000");
}

TEST(TestArithmetic, AddsAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger number = 2;
    EXPECT_EQ(number += 2, 4);
    EXPECT_EQ(number, 4);
}

TEST(TestArithmetic, AddsAndAssignsBigIntegersAndStrings) {
    BigInteger number = 2;
    EXPECT_EQ(number += "2", 4);
    EXPECT_EQ(number, 4);
}

TEST(TestArithmetic, MultipliesBigIntegers) {
    const BigInteger two = 2;
    EXPECT_EQ(two * two, 4);
}

TEST(TestArithmetic, MultipliesVeryBigIntegers) {
    const BigInteger very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number * very_long_number, "1000000000000000000000000000000000000000000");
}

TEST(TestArithmetic, MultipliesBigIntegersAndPrimitiveIntegers) {
    const BigInteger two = 2;
    EXPECT_EQ(two * 2, 4);
    EXPECT_EQ(2 * two, 4);
}

TEST(TestArithmetic, MultipliesBigIntegersAndStrings) {
    const BigInteger two = 2;
    EXPECT_EQ(two * "2", 4);
    EXPECT_EQ("2" * two, 4);
}

TEST(TestArithmetic, MultipliesAndAssignsBigIntegers) {
    BigInteger number = 2;
    EXPECT_EQ(number *= number, 4);
    EXPECT_EQ(number, 4);
}

TEST(TestArithmetic, MultipliesAndAssignsVeryBigIntegers) {
    BigInteger very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number *= very_long_number, "1000000000000000000000000000000000000000000");
    EXPECT_EQ(very_long_number, "1000000000000000000000000000000000000000000");
}

TEST(TestArithmetic, MultipliesAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger number = 2;
    EXPECT_EQ(number *= 2, 4);
    EXPECT_EQ(number, 4);
}

TEST(TestArithmetic, MultipliesAndAssignsBigIntegersAndStrings) {
    BigInteger number = 2;
    EXPECT_EQ(number *= "2", 4);
    EXPECT_EQ(number, 4);
}
