#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestSubtraction, SubtractsBigIntegers) {
    const BigInteger two = 2;
    const BigInteger one = 1;
    EXPECT_EQ(two - two, 0);
    EXPECT_EQ(two - one, 1);
}

TEST(TestSubtraction, SubtractsBigIntegersAndPrimitiveIntegers) {
    const BigInteger two = 2;
    EXPECT_EQ(two - 1, 1);
    EXPECT_EQ(two - 2, 0);
    EXPECT_EQ(2 - two, 0);
    EXPECT_EQ(4 - two, 2);
}

TEST(TestSubtraction, SubtractsBigIntegersAndDigits) {
    EXPECT_EQ(BigInteger(2) - Digit(0), 2);
    EXPECT_EQ(BigInteger(2) - Digit(1), 1);
    EXPECT_EQ(BigInteger(2) - Digit(2), 0);
    EXPECT_EQ(BigInteger(0) - Digit(1), -1);
    EXPECT_EQ(BigInteger("100000000000000000000000000000000000") - Digit(1), "99999999999999999999999999999999999");
}

TEST(TestSubtraction, SubtractsBigIntegersAndStrings) {
    const BigInteger two = 2;
    EXPECT_EQ(two - "1", "1");
    EXPECT_EQ(two - "2", "0");
    EXPECT_EQ("2" - two, "0");
    EXPECT_EQ("4" - two, "2");
}

TEST(TestSubtraction, SubtractsVeryBigIntegers) {
    const BigInteger very_long_number = "1000000000000000000000";
    const BigInteger one = 1;
    EXPECT_EQ(very_long_number - very_long_number, 0);
    EXPECT_EQ(very_long_number - one, "999999999999999999999");
}

TEST(TestSubtraction, SubtractsAndAssignsBigIntegers) {
    BigInteger number = 2;
    BigInteger one = 1;
    EXPECT_EQ(number -= one, 1);
    EXPECT_EQ(number -= number, 0);
    EXPECT_EQ(number, 0);
}

TEST(TestSubtraction, SubtractsAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger number = 2;
    EXPECT_EQ(number -= 1, 1);
    EXPECT_EQ(number -= 1, 0);
    EXPECT_EQ(number, 0);
}

TEST(TestSubtraction, SubtractsAndAssignsBigIntegersAndStrings) {
    BigInteger number = 2;
    EXPECT_EQ(number -= "1", 1);
    EXPECT_EQ(number -= "1", 0);
    EXPECT_EQ(number, 0);
}

TEST(TestSubtraction, SubtractsAndAssignsVeryBigIntegers) {
    BigInteger very_long_number = "100000000000000000000000";
    EXPECT_EQ(very_long_number -= "10000000000000000000000", "90000000000000000000000");
    EXPECT_EQ(very_long_number -= "1000000000000000000000", "89000000000000000000000");
    EXPECT_EQ(very_long_number -= very_long_number, 0);
    EXPECT_EQ(very_long_number, 0);
}
