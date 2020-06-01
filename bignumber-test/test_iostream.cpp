#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestIOStream : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestIOStream, test_types);

TYPED_TEST(TestIOStream, ReadsNumbers) {
    BigInteger<TypeParam> number;
    std::stringstream str;    
    str << "1000 0 -1 25";

    str >> number;
    EXPECT_EQ(number, 1000);

    str >> number;
    EXPECT_EQ(number, 0);

    str >> number;
    EXPECT_EQ(number, -1);

    str >> number;
    EXPECT_EQ(number, 25);
}

TYPED_TEST(TestIOStream, ReadsBigNumbers) {
    BigInteger<TypeParam> number;
    std::stringstream str;
    str << "1000000000000000000000000000000000000000000000 -1000000000000000000000000000000000000000000000";

    str >> number;
    EXPECT_EQ(number, "1000000000000000000000000000000000000000000000");

    str >> number;
    EXPECT_EQ(number, "-1000000000000000000000000000000000000000000000");
}

TYPED_TEST(TestIOStream, WritesNumbers) {
    std::stringstream str;
    BigInteger<TypeParam> eight = 8;
    BigInteger<TypeParam> minus_one = -1;
    BigInteger<TypeParam> nonillion = "1000000000000000000000000000000";
    BigInteger<TypeParam> zero = 0;

    str << eight << ';' << minus_one << ';' << nonillion << ';' << zero;

    EXPECT_EQ(str.str(), "8;-1;1000000000000000000000000000000;0");
}
