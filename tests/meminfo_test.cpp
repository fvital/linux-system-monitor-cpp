
#include <fstream>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include "linux_parser.h"
#include "meminfo.h"

const std::string kFixturePath =
    std::string(TEST_FIXTURE_DIR); // injected via CMake

TEST(MemInfoT, defaultInitIfstreamAssign)
{
    MemInfo m1;
    std::ifstream ifilestream(kFixturePath + LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
    ifilestream >> m1;

    EXPECT_EQ(7802668, m1.mem_total);
    EXPECT_EQ(523696, m1.mem_free);
    EXPECT_EQ(3951188, m1.mem_available);
}

TEST(MemInfoT, ifstreamInit)
{

    std::ifstream ifilestream(kFixturePath + LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
    MemInfo m1{ifilestream};

    EXPECT_EQ(7802668, m1.mem_total);
    EXPECT_EQ(523696, m1.mem_free);
    EXPECT_EQ(3951188, m1.mem_available);
}