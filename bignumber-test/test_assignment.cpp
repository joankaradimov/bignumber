#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestAssignment, AssignmentChangesTheValue) {
    BigInteger number;

    EXPECT_EQ(number, 0);
    number = 8;
    EXPECT_EQ(number, 8);
}

TEST(TestAssignment, Reassignment) {
    BigInteger number;

    EXPECT_EQ(number = 8, 8);
    EXPECT_EQ(number = 10, 10);
    EXPECT_EQ(number = 0, 0);
}

TEST(TestAssignment, StringAssignment) {
    BigInteger number;

    EXPECT_EQ(number = "10", 10);
    EXPECT_EQ(number = "100000000000", 100000000000);
    EXPECT_EQ(number = "100000000000000000000000000000", "100000000000000000000000000000");
}

TEST(TestAssignment, AssignmentCopiesTheValues) {
    BigInteger number;
    BigInteger other = 42;

    EXPECT_EQ(number = other, 42);
    other = 50;
    EXPECT_EQ(number, 42);
    EXPECT_EQ(other, 50);
}

TEST(TestAssignment, AssignmentOfIntegerBounds) {
    BigInteger number;

    EXPECT_EQ(number = std::numeric_limits<unsigned __int8>::max(), "255");
    EXPECT_EQ(number = std::numeric_limits<signed __int8>::max(), "127");
    EXPECT_EQ(number = std::numeric_limits<signed __int8>::min(), "-128");

    EXPECT_EQ(number = std::numeric_limits<unsigned __int16>::max(), "65535");
    EXPECT_EQ(number = std::numeric_limits<signed __int16>::max(), "32767");
    EXPECT_EQ(number = std::numeric_limits<signed __int16>::min(), "-32768");

    EXPECT_EQ(number = std::numeric_limits<unsigned __int32>::max(), "4294967295");
    EXPECT_EQ(number = std::numeric_limits<signed __int32>::max(), "2147483647");
    EXPECT_EQ(number = std::numeric_limits<signed __int32>::min(), "-2147483648");

    EXPECT_EQ(number = std::numeric_limits<unsigned __int64>::max(), "18446744073709551615");
    EXPECT_EQ(number = std::numeric_limits<signed __int64>::max(), "9223372036854775807");
    EXPECT_EQ(number = std::numeric_limits<signed __int64>::min(), "-9223372036854775808");
}
