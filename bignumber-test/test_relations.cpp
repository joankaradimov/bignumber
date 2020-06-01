#include "gtest/gtest.h"

#include "bignumber.hpp"

template<typename T> struct TestRelations : public testing::Test {};
using test_types = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;
TYPED_TEST_CASE(TestRelations, test_types);

TYPED_TEST(TestRelations, LessThan) {
    EXPECT_LT(BigInteger<TypeParam>(0), BigInteger<TypeParam>(1));
    EXPECT_LT(BigInteger<TypeParam>(2), BigInteger<TypeParam>(3));
    EXPECT_LT(BigInteger<TypeParam>("1000000000000000000000"), BigInteger<TypeParam>("1000000000000000000001"));
    EXPECT_LT(BigInteger<TypeParam>(0), "1000000000000000000001");
    EXPECT_LT(0, BigInteger<TypeParam>("1000000000000000000001"));

    EXPECT_FALSE(BigInteger<TypeParam>(0) < BigInteger<TypeParam>(0));
    EXPECT_FALSE(BigInteger<TypeParam>(1) < BigInteger<TypeParam>(0));
    EXPECT_FALSE(BigInteger<TypeParam>("1000000000000000000001") < BigInteger<TypeParam>("1000000000000000000000"));
}

TYPED_TEST(TestRelations, LessThanWithNegative) {
    EXPECT_LT(BigInteger<TypeParam>(-1), BigInteger<TypeParam>(0));
    EXPECT_LT(BigInteger<TypeParam>(-1), BigInteger<TypeParam>(5));
    EXPECT_LT(BigInteger<TypeParam>(-1), BigInteger<TypeParam>("10000000000000000000000000000000000000000"));
    EXPECT_LT(BigInteger<TypeParam>("-10000000000000000000000000000000000000000"), BigInteger<TypeParam>(-1));
    EXPECT_LT(BigInteger<TypeParam>("-1000000000000000000000000000000000000000000000000000000000000000000"), BigInteger<TypeParam>("-10000000000000000000000000000000000000000"));
}

TYPED_TEST(TestRelations, GreaterThan) {
    EXPECT_GT(BigInteger<TypeParam>(1), BigInteger<TypeParam>(0));
    EXPECT_GT(BigInteger<TypeParam>(3), BigInteger<TypeParam>(2));
    EXPECT_GT(BigInteger<TypeParam>("1000000000000000000001"), BigInteger<TypeParam>("1000000000000000000000"));
    EXPECT_GT("1000000000000000000001", BigInteger<TypeParam>(0));
    EXPECT_GT(BigInteger<TypeParam>("1000000000000000000001"), 0);

    EXPECT_FALSE(BigInteger<TypeParam>(0) > BigInteger<TypeParam>(0));
    EXPECT_FALSE(BigInteger<TypeParam>(0) > BigInteger<TypeParam>(1));
    EXPECT_FALSE(BigInteger<TypeParam>("1000000000000000000000") > BigInteger<TypeParam>("1000000000000000000001"));
}

TYPED_TEST(TestRelations, LessThanOrEqual) {
    EXPECT_LE(BigInteger<TypeParam>(0), BigInteger<TypeParam>(1));
    EXPECT_LE(BigInteger<TypeParam>(2), BigInteger<TypeParam>(3));
    EXPECT_LE(BigInteger<TypeParam>(42), BigInteger<TypeParam>(42));
    EXPECT_LE(BigInteger<TypeParam>("1000000000000000000000"), BigInteger<TypeParam>("1000000000000000000000"));
    EXPECT_LE(BigInteger<TypeParam>("1000000000000000000000"), BigInteger<TypeParam>("1000000000000000000001"));
    EXPECT_LE(BigInteger<TypeParam>(0), "1000000000000000000001");
    EXPECT_LE(0, BigInteger<TypeParam>("1000000000000000000001"));

    EXPECT_FALSE(BigInteger<TypeParam>(1) <= BigInteger<TypeParam>(0));
    EXPECT_FALSE(BigInteger<TypeParam>("1000000000000000000001") <= BigInteger<TypeParam>("1000000000000000000000"));
}

TYPED_TEST(TestRelations, GreaterThanOrEqual) {
    EXPECT_GE(BigInteger<TypeParam>(1), BigInteger<TypeParam>(0));
    EXPECT_GE(BigInteger<TypeParam>(3), BigInteger<TypeParam>(2));
    EXPECT_GE(BigInteger<TypeParam>(42), BigInteger<TypeParam>(42));
    EXPECT_GE(BigInteger<TypeParam>("1000000000000000000000"), BigInteger<TypeParam>("1000000000000000000000"));
    EXPECT_GE(BigInteger<TypeParam>("1000000000000000000001"), BigInteger<TypeParam>("1000000000000000000000"));
    EXPECT_GE("1000000000000000000001", BigInteger<TypeParam>(0));
    EXPECT_GE(BigInteger<TypeParam>("1000000000000000000001"), 0);

    EXPECT_FALSE(BigInteger<TypeParam>(0) >= BigInteger<TypeParam>(1));
    EXPECT_FALSE(BigInteger<TypeParam>("1000000000000000000000") >= BigInteger<TypeParam>("1000000000000000000001"));
}

TYPED_TEST(TestRelations, Equals) {
    EXPECT_EQ(BigInteger<TypeParam>(0), BigInteger<TypeParam>(0));
    EXPECT_EQ(BigInteger<TypeParam>("1000000000000000000000"), BigInteger<TypeParam>("1000000000000000000000"));
    EXPECT_EQ(BigInteger<TypeParam>(100000000), "100000000"); // TODO: use a longer number here
    EXPECT_EQ(100000000, BigInteger<TypeParam>("100000000")); // TODO: use a longer number here

    EXPECT_FALSE(BigInteger<TypeParam>(0) == BigInteger<TypeParam>(1));
    EXPECT_FALSE(BigInteger<TypeParam>("1000000000000000000001") == BigInteger<TypeParam>("1000000000000000000000"));
}

TYPED_TEST(TestRelations, NotEquals) {
    EXPECT_NE(BigInteger<TypeParam>(0), BigInteger<TypeParam>(1));
    EXPECT_NE(BigInteger<TypeParam>("1000000000000000000000"), BigInteger<TypeParam>("1000000000000000000001"));
    EXPECT_NE(BigInteger<TypeParam>(100000000), "100000001"); // TODO: use a longer number here
    EXPECT_NE(100000000, BigInteger<TypeParam>("100000001")); // TODO: use a longer number here

    EXPECT_FALSE(BigInteger<TypeParam>(0) != BigInteger<TypeParam>(0));
    EXPECT_FALSE(BigInteger<TypeParam>("1000000000000000000000") != BigInteger<TypeParam>("1000000000000000000000"));
}
