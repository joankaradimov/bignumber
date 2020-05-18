#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestDivision, DividesBigIntegers) {
    const BigInteger four = 4;
    const BigInteger two = 2;
    EXPECT_EQ(four / four, 1);
    EXPECT_EQ(four / two, 2);
}

TEST(TestDivision, DividesBigIntegersAndPrimitiveIntegers) {
    const BigInteger four = 4;
    EXPECT_EQ(four / 2, 2);
    EXPECT_EQ(four / 1, 4);
    EXPECT_EQ(8 / four, 2);
    EXPECT_EQ(400 / four, 100);
}

TEST(TestDivision, DividesBigIntegersAndStrings) {
    const BigInteger four = 4;
    EXPECT_EQ(four / "2", "2");
    EXPECT_EQ(four / "1", "4");
    EXPECT_EQ("8" / four, "2");
    EXPECT_EQ("400" / four, 100);
}

TEST(TestDivision, DividesVeryBigIntegers) {
    const BigInteger very_long_number = "1000000000000000000000000000000000000000000";
    const BigInteger another_long_number = "1000000000000000000000000";
    EXPECT_EQ(very_long_number / another_long_number, "1000000000000000000");
}

TEST(TestDivision, DividesAndRoundsDown) {
    const BigInteger very_long_number = "1000000000000000000000000000000000000000000";
    EXPECT_EQ(very_long_number / "33333333333333333333333333333", "30000000000000");
    EXPECT_EQ(very_long_number / 3, "333333333333333333333333333333333333333333");

    EXPECT_EQ(BigInteger(1) / 2, 0);
}

TEST(TestDivision, DividesAndAssignsBigIntegers) {
    BigInteger number = 4;
    BigInteger two = 2;
    EXPECT_EQ(number /= two, 2);
    EXPECT_EQ(number /= two, 1);
    EXPECT_EQ(number, 1);
}

TEST(TestDivision, DividesAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger number = 4;
    EXPECT_EQ(number /= 2, 2);
    EXPECT_EQ(number /= 2, 1);
    EXPECT_EQ(number, 1);
}

TEST(TestDivision, DividesAndAssignsBigIntegersAndStrings) {
    BigInteger number = 4;
    EXPECT_EQ(number /= "2", 2);
    EXPECT_EQ(number /= "2", 1);
    EXPECT_EQ(number, 1);
}

TEST(TestDivision, DividesAndAssignsVeryBigIntegers) {
    BigInteger very_long_number = "1000000000000000000000000000000000000000000000000000";
    EXPECT_EQ(very_long_number /= "1000000000000000000000", "1000000000000000000000000000000");
    EXPECT_EQ(very_long_number /= very_long_number, 1);
    EXPECT_EQ(very_long_number, 1);
}
