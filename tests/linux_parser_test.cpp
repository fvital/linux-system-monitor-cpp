
#include <string>

#include <gtest/gtest.h>

#include "linux_parser.h"

const std::string kFixturePath =
    std::string(TEST_FIXTURE_DIR); // injected via CMake

TEST(LinuxParserT, OperatingSystem1)
{
    EXPECT_FALSE(LinuxParser::OperatingSystem().empty());
}
TEST(LinuxParserT, OperatingSystem2)
{
    EXPECT_EQ("Ubuntu 22.04.5 LTS", LinuxParser::OperatingSystem(TEST_FIXTURE_DIR));
}

TEST(LinuxParserT, Kernel1)
{
    EXPECT_FALSE(LinuxParser::Kernel().empty());
}

TEST(LinuxParserT, Kernel2)
{
    EXPECT_EQ("5.15.148-tegra", LinuxParser::Kernel(TEST_FIXTURE_DIR));
}

TEST(LinuxParserT, Pids1)
{
    EXPECT_FALSE(LinuxParser::Pids().empty());
}

TEST(LinuxParserT, Pids2)
{
    EXPECT_EQ(2, LinuxParser::Pids(TEST_FIXTURE_DIR).size());
}

TEST(LinuxParserT, UpTime)
{
    EXPECT_FLOAT_EQ(453078.54, LinuxParser::UpTime(TEST_FIXTURE_DIR));
}

TEST(LinuxParserT, Jiffies)
{
    EXPECT_EQ(266793352, LinuxParser::Jiffies(TEST_FIXTURE_DIR));
}

TEST(LinuxParserT, ActiveJiffies)
{
    EXPECT_EQ(2958349, LinuxParser::ActiveJiffies(TEST_FIXTURE_DIR));
}

TEST(LinuxParserT, IdleJiffies)
{
    EXPECT_EQ(263835003, LinuxParser::IdleJiffies(TEST_FIXTURE_DIR));
}

TEST(LinuxParserT, JiffiesConsistent)
{
    EXPECT_EQ(
        LinuxParser::Jiffies(TEST_FIXTURE_DIR),
        LinuxParser::ActiveJiffies(TEST_FIXTURE_DIR) + LinuxParser::IdleJiffies(TEST_FIXTURE_DIR));
}

TEST(LinuxParserT, TotalProcesses1)
{
    EXPECT_EQ(2, LinuxParser::TotalProcesses(TEST_FIXTURE_DIR));
}
TEST(LinuxParserT, TotalProcesses2)
{
    EXPECT_EQ(LinuxParser::Pids().size(), LinuxParser::TotalProcesses());
}