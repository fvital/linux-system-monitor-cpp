
#include <string>

#include <gtest/gtest.h>

#include "linux_parser.h"
#include "system.h"

const std::string kFixturePath = std::string(TEST_FIXTURE_DIR); // injected via CMake

class SystemTest : public testing::Test
{
protected:
    SystemTest() : s1(kFixturePath) {};
    System s1;
};

TEST_F(SystemTest, UserMap)
{
    EXPECT_EQ("root", s1.User(0));
    EXPECT_EQ("u1", s1.User(1001));
    EXPECT_EQ("u2", s1.User(1002));
}

TEST_F(SystemTest, UpTime)
{
    EXPECT_EQ(453079, s1.UpTime());
}

TEST_F(SystemTest, TotalProcesses)
{
    EXPECT_EQ(2, s1.TotalProcesses());
}

TEST_F(SystemTest, Processes1)
{
    EXPECT_EQ(2, s1.Processes().size());
}

TEST_F(SystemTest, Processes2)
{
    EXPECT_EQ(2599, s1.Processes()[1].Pid());
    EXPECT_EQ(1234, s1.Processes()[0].Pid());
    EXPECT_EQ(1002, s1.Processes()[0].Uid());
}

TEST_F(SystemTest, MemoryUtilization)
{
    EXPECT_FLOAT_EQ(0.932882445, s1.MemoryUtilization());
}

TEST_F(SystemTest, OperatingSystem)
{
    EXPECT_EQ("Test Ubuntu 22.04.5 LTS", s1.OperatingSystem());
}

TEST_F(SystemTest, Kernel)
{
    EXPECT_EQ("0.0.0-kernelTest", s1.Kernel());
}