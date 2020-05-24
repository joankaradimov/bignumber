#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestMultiplication, MultipliesBigIntegers) {
    const BigInteger two = 2;
    EXPECT_EQ(two * two, 4);
}

TEST(TestMultiplication, MultipliesBigIntegersAndPrimitiveIntegers) {
    const BigInteger two = 2;
    EXPECT_EQ(two * 2, 4);
    EXPECT_EQ(2 * two, 4);

    EXPECT_EQ(two * 5, 10);
    EXPECT_EQ(5 * two, 10);
}

TEST(TestMultiplication, MultipliesBigIntegersAndStrings) {
    const BigInteger two = 2;
    EXPECT_EQ(two * "2", 4);
    EXPECT_EQ("2" * two, 4);

    EXPECT_EQ(two * "5", 10);
    EXPECT_EQ("5" * two, 10);
}

TEST(TestMultiplication, MultipliesVeryBigIntegers) {
    const BigInteger very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number * very_long_number, "1000000000000000000000000000000000000000000");
    EXPECT_EQ(very_long_number * "500000000000000000000000000", "500000000000000000000000000000000000000000000000");
    EXPECT_EQ("600000000000000000000000000" * very_long_number, "600000000000000000000000000000000000000000000000");
}


TEST(TestMultiplication, MultipliesAndAssignsBigIntegers) {
    BigInteger number = 2;
    EXPECT_EQ(number *= number, 4);
    EXPECT_EQ(number, 4);
}

TEST(TestMultiplication, MultipliesAndAssignsBigIntegersAndPrimitiveIntegers) {
    BigInteger number = 2;
    EXPECT_EQ(number *= 2, 4);
    EXPECT_EQ(number, 4);
}

TEST(TestMultiplication, MultipliesAndAssignsBigIntegersAndStrings) {
    BigInteger number = 2;
    EXPECT_EQ(number *= "2", 4);
    EXPECT_EQ(number, 4);
}

TEST(TestMultiplication, MultipliesAndAssignsVeryBigIntegers) {
    BigInteger very_long_number = "1000000000000000000000";
    EXPECT_EQ(very_long_number *= very_long_number, "1000000000000000000000000000000000000000000");
    EXPECT_EQ(very_long_number, "1000000000000000000000000000000000000000000");
}
