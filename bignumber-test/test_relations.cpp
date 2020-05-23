#include "gtest/gtest.h"

#include "bignumber.hpp"

TEST(TestRelations, LessThan) {
    EXPECT_LT(BigInteger(0), BigInteger(1));
    EXPECT_LT(BigInteger(2), BigInteger(3));
    EXPECT_LT(BigInteger("1000000000000000000000"), BigInteger("1000000000000000000001"));
    EXPECT_LT(BigInteger(0), "1000000000000000000001");
    EXPECT_LT(0, BigInteger("1000000000000000000001"));

    EXPECT_FALSE(BigInteger(0) < BigInteger(0));
    EXPECT_FALSE(BigInteger(1) < BigInteger(0));
    EXPECT_FALSE(BigInteger("1000000000000000000001") < BigInteger("1000000000000000000000"));
}

TEST(TestRelations, LessThanWithNegative) {
    EXPECT_LT(BigInteger(-1), BigInteger(0));
    EXPECT_LT(BigInteger(-1), BigInteger(5));
    EXPECT_LT(BigInteger(-1), BigInteger("10000000000000000000000000000000000000000"));
    EXPECT_LT(BigInteger("-10000000000000000000000000000000000000000"), BigInteger(-1));
    EXPECT_LT(BigInteger("-1000000000000000000000000000000000000000000000000000000000000000000"), BigInteger("-10000000000000000000000000000000000000000"));
}

TEST(TestRelations, GreaterThan) {
    EXPECT_GT(BigInteger(1), BigInteger(0));
    EXPECT_GT(BigInteger(3), BigInteger(2));
    EXPECT_GT(BigInteger("1000000000000000000001"), BigInteger("1000000000000000000000"));
    EXPECT_GT("1000000000000000000001", BigInteger(0));
    EXPECT_GT(BigInteger("1000000000000000000001"), 0);

    EXPECT_FALSE(BigInteger(0) > BigInteger(0));
    EXPECT_FALSE(BigInteger(0) > BigInteger(1));
    EXPECT_FALSE(BigInteger("1000000000000000000000") > BigInteger("1000000000000000000001"));
}

TEST(TestRelations, LessThanOrEqual) {
    EXPECT_LE(BigInteger(0), BigInteger(1));
    EXPECT_LE(BigInteger(2), BigInteger(3));
    EXPECT_LE(BigInteger(42), BigInteger(42));
    EXPECT_LE(BigInteger("1000000000000000000000"), BigInteger("1000000000000000000000"));
    EXPECT_LE(BigInteger("1000000000000000000000"), BigInteger("1000000000000000000001"));
    EXPECT_LE(BigInteger(0), "1000000000000000000001");
    EXPECT_LE(0, BigInteger("1000000000000000000001"));

    EXPECT_FALSE(BigInteger(1) <= BigInteger(0));
    EXPECT_FALSE(BigInteger("1000000000000000000001") <= BigInteger("1000000000000000000000"));
}

TEST(TestRelations, GreaterThanOrEqual) {
    EXPECT_GE(BigInteger(1), BigInteger(0));
    EXPECT_GE(BigInteger(3), BigInteger(2));
    EXPECT_GE(BigInteger(42), BigInteger(42));
    EXPECT_GE(BigInteger("1000000000000000000000"), BigInteger("1000000000000000000000"));
    EXPECT_GE(BigInteger("1000000000000000000001"), BigInteger("1000000000000000000000"));
    EXPECT_GE("1000000000000000000001", BigInteger(0));
    EXPECT_GE(BigInteger("1000000000000000000001"), 0);

    EXPECT_FALSE(BigInteger(0) >= BigInteger(1));
    EXPECT_FALSE(BigInteger("1000000000000000000000") >= BigInteger("1000000000000000000001"));
}

TEST(TestRelations, Equals) {
    EXPECT_EQ(BigInteger(0), BigInteger(0));
    EXPECT_EQ(BigInteger("1000000000000000000000"), BigInteger("1000000000000000000000"));
    EXPECT_EQ(BigInteger(100000000), "100000000"); // TODO: use a longer number here
    EXPECT_EQ(100000000, BigInteger("100000000")); // TODO: use a longer number here

    EXPECT_FALSE(BigInteger(0) == BigInteger(1));
    EXPECT_FALSE(BigInteger("1000000000000000000001") == BigInteger("1000000000000000000000"));
}

TEST(TestRelations, NotEquals) {
    EXPECT_NE(BigInteger(0), BigInteger(1));
    EXPECT_NE(BigInteger("1000000000000000000000"), BigInteger("1000000000000000000001"));
    EXPECT_NE(BigInteger(100000000), "100000001"); // TODO: use a longer number here
    EXPECT_NE(100000000, BigInteger("100000001")); // TODO: use a longer number here

    EXPECT_FALSE(BigInteger(0) != BigInteger(0));
    EXPECT_FALSE(BigInteger("1000000000000000000000") != BigInteger("1000000000000000000000"));
}
