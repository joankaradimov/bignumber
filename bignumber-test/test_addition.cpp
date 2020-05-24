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
