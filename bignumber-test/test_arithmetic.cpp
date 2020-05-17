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

TEST(TestArithmetic, SubtractsBigIntegers) {
    const BigInteger two = 2;
    const BigInteger one = 1;
    EXPECT_EQ(two - two, 0);
    EXPECT_EQ(two - one, 1);
}

TEST(TestArithmetic, SubtractsVeryBigIntegers) {
    const BigInteger very_long_number = "1000000000000000000000";
    const BigInteger one = 1;
    EXPECT_EQ(very_long_number - very_long_number, 0);
    EXPECT_EQ(very_long_number - one, "999999999999999999999");
}

TEST(TestArithmetic, SubtractsBigIntegersAndPrimitiveIntegers) {
    const BigInteger two = 2;
    EXPECT_EQ(two - 1, 1);
    EXPECT_EQ(two - 2, 0);
    EXPECT_EQ(2 - two, 0);
    EXPECT_EQ(4 - two, 2);
}

TEST(TestArithmetic, SubtractsBigIntegersAndStrings) {
    const BigInteger two = 2;
    EXPECT_EQ(two - "1", "1");
    EXPECT_EQ(two - "2", "0");
    EXPECT_EQ("2" - two, "0");
    EXPECT_EQ("4" - two, "2");
}

TEST(TestArithmetic, SubtractsAndAssignsBigIntegers) {
    BigInteger number = 2;
    BigInteger one = 1;
    EXPECT_EQ(number -= one, 1);
    EXPECT_EQ(number -= number, 0);
    EXPECT_EQ(number, 0);
}

TEST(TestArithmetic, SubtractsAndAssignsVeryBigIntegers) {
    BigInteger very_long_number = "100000000000000000000000";
    EXPECT_EQ(very_long_number -= "10000000000000000000000", "90000000000000000000000");
    EXPECT_EQ(very_long_number -= "1000000000000000000000", "89000000000000000000000");
    EXPECT_EQ(very_long_number -= very_long_number, 0);
    EXPECT_EQ(very_long_number, 0);
}

TEST(TestArithmetic, SubtractsAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger number = 2;
    EXPECT_EQ(number -= 1, 1);
    EXPECT_EQ(number -= 1, 0);
    EXPECT_EQ(number, 0);
}

TEST(TestArithmetic, SubtractsAndAssignsBigIntegersAndStrings) {
    BigInteger number = 2;
    EXPECT_EQ(number -= "1", 1);
    EXPECT_EQ(number -= "1", 0);
    EXPECT_EQ(number, 0);
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
