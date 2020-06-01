#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestConstruction : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestConstruction, test_types);

TYPED_TEST(TestConstruction, ConstructsZerosByDefault) {
    BigInteger<TypeParam> empty;
    EXPECT_EQ(empty, 0);
}

TYPED_TEST(TestConstruction, ConstructsFromIntegers) {
    BigInteger<TypeParam> zero = 0;
    EXPECT_EQ(zero, 0);

    BigInteger<TypeParam> one = 1;
    EXPECT_EQ(one, 1);
}

TYPED_TEST(TestConstruction, ConstructsFromDecimalStrings) {
    BigInteger<TypeParam> zero = "0";
    EXPECT_EQ(zero, 0);

    BigInteger<TypeParam> hundred = "100";
    EXPECT_EQ(hundred, 100);

    BigInteger<TypeParam> billion = "1000000000";
    EXPECT_EQ(billion, 1000000000);

    BigInteger<TypeParam> c = "299792458";
    EXPECT_EQ(c, 299792458);

    BigInteger<TypeParam> bigger_than_int_64 = "1000000000000000000000";
    EXPECT_EQ(bigger_than_int_64, "1000000000000000000000");
}

TYPED_TEST(TestConstruction, ConstructsFromBinaryStrings) {
    EXPECT_EQ(BigInteger<TypeParam>("0b0"), 0);
    EXPECT_EQ(BigInteger<TypeParam>("0b1"), 1);
    EXPECT_EQ(BigInteger<TypeParam>("0b1000"), 8);
    EXPECT_EQ(BigInteger<TypeParam>("-0b1000"), -8);
    EXPECT_EQ(BigInteger<TypeParam>("0b1000000000000000000000000000000000000000000000000000000000000"), "1152921504606846976");
}

TYPED_TEST(TestConstruction, ConstructsFromHexadecimalStrings) {
    EXPECT_EQ(BigInteger<TypeParam>("0x0"), 0);
    EXPECT_EQ(BigInteger<TypeParam>("0x1"), 1);
    EXPECT_EQ(BigInteger<TypeParam>("0xa"), 10);
    EXPECT_EQ(BigInteger<TypeParam>("0xA"), 10);
    EXPECT_EQ(BigInteger<TypeParam>("0xF"), 15);
    EXPECT_EQ(BigInteger<TypeParam>("0xFF"), 255);
    EXPECT_EQ(BigInteger<TypeParam>("-0xFF"), -255);
    EXPECT_EQ(BigInteger<TypeParam>("0x1000"), 4096);
    EXPECT_EQ(BigInteger<TypeParam>("0xdefec8"), 14614216);
    EXPECT_EQ(BigInteger<TypeParam>("0xCafeBabe"), 3405691582);
    EXPECT_EQ(BigInteger<TypeParam>("0xDEADBEEF"), 3735928559);
    EXPECT_EQ(BigInteger<TypeParam>("0xDEADBEEF"), 3735928559);
    EXPECT_EQ(BigInteger<TypeParam>("0x1000000000000000000000000000000000000000000"), "374144419156711147060143317175368453031918731001856");
}

TYPED_TEST(TestConstruction, ConstructsFromOctalStrings) {
    EXPECT_EQ(BigInteger<TypeParam>("00"), 0);
    EXPECT_EQ(BigInteger<TypeParam>("01"), 1);
    EXPECT_EQ(BigInteger<TypeParam>("07"), 7);
    EXPECT_EQ(BigInteger<TypeParam>("010"), 8);
}

TYPED_TEST(TestConstruction, ConstructsFromOtherBigIntegers) {
    BigInteger<TypeParam> one = 1;
    BigInteger<TypeParam> another_one = one;
    EXPECT_EQ(one, another_one);
}

TYPED_TEST(TestConstruction, ConstructsFromIntegerBounds) {
    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<unsigned __int8>::max()), "255");
    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<signed __int8>::max()), "127");
    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<signed __int8>::min()), "-128");

    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<unsigned __int16>::max()), "65535");
    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<signed __int16>::max()), "32767");
    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<signed __int16>::min()), "-32768");

    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<unsigned __int32>::max()), "4294967295");
    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<signed __int32>::max()), "2147483647");
    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<signed __int32>::min()), "-2147483648");

    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<unsigned __int64>::max()), "18446744073709551615");
    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<signed __int64>::max()), "9223372036854775807");
    EXPECT_EQ(BigInteger<TypeParam>(std::numeric_limits<signed __int64>::min()), "-9223372036854775808");
}

TYPED_TEST(TestConstruction, ConstructsLeadingTwosComplementDigits) {
    EXPECT_EQ(BigInteger<TypeParam>(signed __int64(-1)), -1);
    EXPECT_EQ(BigInteger<TypeParam>(signed __int64(std::numeric_limits<signed __int32>::min())), "-2147483648");
    EXPECT_EQ(BigInteger<TypeParam>(signed __int64(0xffffffff)), "4294967295");
    EXPECT_EQ(BigInteger<TypeParam>(signed __int64(0xffffffff00000000)), "-4294967296");
}
