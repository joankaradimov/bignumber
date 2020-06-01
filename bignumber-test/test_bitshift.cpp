#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestBitshift : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestBitshift, test_types);

TYPED_TEST(TestBitshift, ShortShifts) {
    EXPECT_EQ(BigInteger<TypeParam> (1) << 6, 64);
    EXPECT_EQ(BigInteger<TypeParam>(7) << 2, 28);

    EXPECT_EQ(BigInteger<TypeParam>(64) >> 6, 1);
    EXPECT_EQ(BigInteger<TypeParam>(64) >> 5, 2);
    EXPECT_EQ(BigInteger<TypeParam>(42) >> 1, 21);
    EXPECT_EQ(BigInteger<TypeParam>(3) >> 1, 1);
    EXPECT_EQ(BigInteger<TypeParam>(15) >> 3, 1);
}

TYPED_TEST(TestBitshift, ShiftByZeroBits) {
    EXPECT_EQ(BigInteger<TypeParam>(1) << 0, 1);
    EXPECT_EQ(BigInteger<TypeParam>(42) << 0, 42);

    EXPECT_EQ(BigInteger<TypeParam>(1) >> 0, 1);
    EXPECT_EQ(BigInteger<TypeParam>(42) >> 0, 42);
}

TYPED_TEST(TestBitshift, ShiftZeroValues) {
    EXPECT_EQ(BigInteger<TypeParam>(0) << 6, 0);
    EXPECT_EQ(BigInteger<TypeParam>(0) << 1000, 0);

    EXPECT_EQ(BigInteger<TypeParam>(0) >> 8, 0);
    EXPECT_EQ(BigInteger<TypeParam>(0) >> 1000, 0);
}

TYPED_TEST(TestBitshift, LongShifts) {
    EXPECT_EQ(BigInteger<TypeParam>(5) << 120, "6646139978924579364519035301401722880");
    EXPECT_EQ(BigInteger<TypeParam>(1) << 200, "1606938044258990275541962092341162602522202993782792835301376");

    EXPECT_EQ(BigInteger<TypeParam>("6646139978924579364519035301401722880") >> 120, 5);
    EXPECT_EQ(BigInteger<TypeParam>("1606938044258990275541962092341162602522202993782792835301376") >> 200, 1);
}

TYPED_TEST(TestBitshift, ShiftBigNumbersByLittle) {
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") << 0, "1000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") << 1, "2000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") << 10, "1024000000000000000000000000");

    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") >> 0, "1000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") >> 1, "500000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("1024000000000000000000000") >> 10, "1000000000000000000000");
}

TYPED_TEST(TestBitshift, ShiftBigNumbersByALot) {
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") << 15, "32768000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") << 17, "131072000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") << 31, "2147483648000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") << 33, "8589934592000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") << 63, "9223372036854775808000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000000") << 65, "36893488147419103232000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("10000000000000000000000000000") << 200, "16069380442589902755419620923411626025222029937827928353013760000000000000000000000000000");

    EXPECT_EQ(BigInteger<TypeParam>("32768000000000000000000000000") >> 15, "1000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("131072000000000000000000000000") >> 17, "1000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("2147483648000000000000000000000000") >> 31, "1000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("8589934592000000000000000000000000") >> 33, "1000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("9223372036854775808000000000000000000000000") >> 63, "1000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("36893488147419103232000000000000000000000000") >> 65, "1000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("16069380442589902755419620923411626025222029937827928353013760000000000000000000000000000") >> 200, "10000000000000000000000000000");
    EXPECT_EQ(BigInteger<TypeParam>("16069380442589902755419620923411626025222029937827928353013760000000000000000000000000000") >> 227, "74505805969238281250");
    EXPECT_EQ(BigInteger<TypeParam>("16069380442589902755419620923411626025222029937827928353013760000000000000000000000000000") >> 293, 1);
}

TYPED_TEST(TestBitshift, Assignments) {
    BigInteger<TypeParam> first_number = 1;
    EXPECT_EQ(first_number <<= 6, 64);
    EXPECT_EQ(first_number <<= 10, 65536);
    EXPECT_EQ(first_number <<= 100, "83076749736557242056487941267521536");

    BigInteger<TypeParam> second_number = "1000000000000000000000000000000000000";
    EXPECT_EQ(second_number <<= 65, "36893488147419103232000000000000000000000000000000000000");
    
    BigInteger<TypeParam> third_number = "10000000000000000000000000000000000000000000";
    EXPECT_EQ(second_number >>= 10, "36028797018963968000000000000000000000000000000000000");
    EXPECT_EQ(second_number >>= 65, "976562500000000000000000000000000");
    EXPECT_EQ(second_number >>= 90, "788860");
    EXPECT_EQ(second_number >>= 19, 1);
}
