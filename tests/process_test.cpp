#include <gtest/gtest.h>
#include <map>
#include <string>
#include <unistd.h>

#include "process.h"

const std::string kFixturePath = std::string(TEST_FIXTURE_DIR); // injected via CMake

class ProcessTest : public testing::Test
{
protected:
    ProcessTest() : test_map({{0, "root"},
                              {1001, "u1"},
                              {1002, "u2"}}) {};
    std::map<int, std::string> test_map;
    double sys_uptime_ = 453078.54;
};

TEST_F(ProcessTest, StatusParse1)
{
    Process p1{2599, test_map, sys_uptime_, kFixturePath};

    EXPECT_EQ(2599, p1.Pid());
    EXPECT_EQ(1001, p1.Uid());
    EXPECT_EQ("u1", p1.User());
}

TEST_F(ProcessTest, StatusParse2)
{
    Process p1{1234, test_map, sys_uptime_, kFixturePath};

    EXPECT_EQ(1234, p1.Pid());
    EXPECT_EQ(1002, p1.Uid());
    EXPECT_EQ("u2", p1.User());
}

TEST_F(ProcessTest, StatParseUpTime)
{
    Process p1{2599, test_map, sys_uptime_, kFixturePath};

    EXPECT_EQ((453078.54 - static_cast<double>(3390) / sysconf(_SC_CLK_TCK)), p1.UpTime());
}

TEST_F(ProcessTest, StatParseUtilization)
{
    Process p1{2599, test_map, sys_uptime_, kFixturePath};

    EXPECT_FLOAT_EQ(1.0 / sysconf(_SC_CLK_TCK) / 453044.64, p1.CpuUtilization());
}

TEST_F(ProcessTest, Sort)
{
    Process p1{2599, test_map, sys_uptime_, kFixturePath};
    Process p2{1234, test_map, sys_uptime_, kFixturePath};

    EXPECT_TRUE(p2 < p1);
}

TEST_F(ProcessTest, Ram)
{
    Process p1{2599, test_map, sys_uptime_, kFixturePath};

    EXPECT_EQ("230.77", p1.Ram());
    EXPECT_EQ("230.77", p1.Ram(2));
    EXPECT_EQ("230.770", p1.Ram(3));
    EXPECT_EQ("230.8", p1.Ram(1));
    EXPECT_EQ("231", p1.Ram(0));
}

TEST_F(ProcessTest, Command)
{
    Process p1{2599, test_map, sys_uptime_, kFixturePath};

    EXPECT_EQ("./test/path/to/process/cmd", p1.Command());
}