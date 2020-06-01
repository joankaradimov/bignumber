#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestAssignment : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestAssignment, test_types);

TYPED_TEST(TestAssignment, AssignmentChangesTheValue) {
    BigInteger<TypeParam> number;

    EXPECT_EQ(number, 0);
    number = 8;
    EXPECT_EQ(number, 8);
}

TYPED_TEST(TestAssignment, Reassignment) {
    BigInteger<TypeParam> number;

    EXPECT_EQ(number = 8, 8);
    EXPECT_EQ(number = 10, 10);
    EXPECT_EQ(number = 0, 0);
}

TYPED_TEST(TestAssignment, StringAssignment) {
    BigInteger<TypeParam> number;

    EXPECT_EQ(number = "10", 10);
    EXPECT_EQ(number = "100000000000", 100000000000);
    EXPECT_EQ(number = "100000000000000000000000000000", "100000000000000000000000000000");
}

TYPED_TEST(TestAssignment, AssignmentCopiesTheValues) {
    BigInteger<TypeParam> number;
    BigInteger<TypeParam> other = 42;

    EXPECT_EQ(number = other, 42);
    other = 50;
    EXPECT_EQ(number, 42);
    EXPECT_EQ(other, 50);
}

TYPED_TEST(TestAssignment, AssignmentOfIntegerBounds) {
    BigInteger<TypeParam> number;

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
