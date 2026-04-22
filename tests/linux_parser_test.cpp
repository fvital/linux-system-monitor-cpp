#include <gtest/gtest.h>

#include "linux_parser.h"

TEST(LinuxParserT, OperatingSystem)
{
    EXPECT_FALSE(LinuxParser::OperatingSystem().empty());
}