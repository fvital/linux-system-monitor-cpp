#include <gtest/gtest.h>

#include "format.h"

TEST(FormatT, test1)
{

    EXPECT_EQ("00:00:00", Format::ElapsedTime(0));
    EXPECT_EQ("00:00:01", Format::ElapsedTime(1));
    EXPECT_EQ("00:00:10", Format::ElapsedTime(10));
    EXPECT_EQ("00:01:00", Format::ElapsedTime(60));
    EXPECT_EQ("00:01:10", Format::ElapsedTime(70));
    EXPECT_EQ("00:10:01", Format::ElapsedTime(601));
    EXPECT_EQ("01:00:01", Format::ElapsedTime(3601));
    EXPECT_EQ("02:00:00", Format::ElapsedTime(7200));
    EXPECT_EQ("200:00:01", Format::ElapsedTime(720001));
}