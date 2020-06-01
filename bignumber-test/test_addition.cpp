#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestAddition, AddsBigIntegers) {
    const BigInteger two = 2;
    EXPECT_EQ(two + two, 4);
}

TEST(TestAddition, AddsBigIntegersAndPrimitiveIntegers) {
    const BigInteger two = 2;
    EXPECT_EQ(two + 2, 4);
    EXPECT_EQ(2 + two, 4);

    EXPECT_EQ(two + 8, 10);
    EXPECT_EQ(8 + two, 10);
}

TEST(TestAddition, AddsBigIntegersAndDigits) {
    EXPECT_EQ(BigInteger(2) + Digit(0), 2);
    EXPECT_EQ(Digit(0) + BigInteger(2), 2);

    EXPECT_EQ(BigInteger(2) + Digit(2), 4);
    EXPECT_EQ(Digit(2) + BigInteger(2), 4);

    EXPECT_EQ(BigInteger(2) + Digit(8), 10);
    EXPECT_EQ(Digit(8) + BigInteger(2), 10);

    EXPECT_EQ(BigInteger(-1) + Digit(1), 0);
    EXPECT_EQ(BigInteger("100000000000000000000000000000000000") + Digit(1), "100000000000000000000000000000000001");
}

TEST(TestAddition, AddsBigIntegersAndStrings) {
    const BigInteger two = 2;
    EXPECT_EQ(two + "2", 4);
    EXPECT_EQ("2" + two, 4);

    EXPECT_EQ(two + "8", 10);
    EXPECT_EQ("8" + two, 10);
}

TEST(TestAddition, AddsVeryBigIntegers) {
    const BigInteger very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number + very_long_number, "2000000000000000000000");

    EXPECT_EQ(very_long_number + "500000000000000000000000000000000000000000000000000000", "500000000000000000000000000000001000000000000000000000");
    EXPECT_EQ("600000000000000000000000000000000000000000000000000000" + very_long_number, "600000000000000000000000000000001000000000000000000000");
}

TEST(TestAddition, AddsAndAssignsBigIntegers) {
    BigInteger number = 2;
    EXPECT_EQ(number += number, 4);
    EXPECT_EQ(number, 4);
}

TEST(TestAddition, AddsAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger number = 2;
    EXPECT_EQ(number += 2, 4);
    EXPECT_EQ(number, 4);
}

TEST(TestAddition, AddsAndAssignsBigIntegersAndStrings) {
    BigInteger number = 2;
    EXPECT_EQ(number += "2", 4);
    EXPECT_EQ(number, 4);
}

TEST(TestAddition, AddsAndAssignsVeryBigIntegers) {
    BigInteger very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number += very_long_number, "2000000000000000000000");
    EXPECT_EQ(very_long_number, "2000000000000000000000");
}
