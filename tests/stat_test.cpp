#include <string>
#include <fstream>
#include <sstream>

#include <gtest/gtest.h>

#include "linux_parser.h"
#include "stat.h"

const std::string kFixturePath = std::string(TEST_FIXTURE_DIR); // injected via CMake

TEST(CPUStatParse, cpu_total)
{
    CpuStat cpu_stat;
    std::string name;
    std::string filepath = kFixturePath + LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
    std::string line;
    std::ifstream stream(filepath);

    if (stream.is_open())
    {
        while (std::getline(stream, line))
        {
            std::istringstream linestream(line);
            linestream >> name;
            if (name == "cpu")
            {
                linestream >> cpu_stat;
                break;
            }
        }
    }

    EXPECT_EQ(1386404, cpu_stat.user);
    EXPECT_EQ(16016, cpu_stat.nice);
    EXPECT_EQ(1089961, cpu_stat.system);
}

TEST(ProcStatParse, test1)
{
    ProcessStat proc_stat;
    proc_stat.set_sys_uptime(LinuxParser::UpTime(kFixturePath));
    const int test_pid = 2599;
    std::string filepath = kFixturePath + LinuxParser::kProcDirectory + std::to_string(test_pid) + LinuxParser::kStatFilename;
    std::string line;
    std::ifstream stream(filepath);

    if (stream.is_open())
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> proc_stat;
    }

    EXPECT_EQ(1, proc_stat.utime());
    EXPECT_EQ(0, proc_stat.stime());
    EXPECT_EQ(0, proc_stat.cutime());
    EXPECT_EQ(0, proc_stat.cstime());
    EXPECT_EQ(1, proc_stat.total_time());
}

TEST(ProcStatParse, test2)
{
    ProcessStat proc_stat;
    proc_stat.set_sys_uptime(LinuxParser::UpTime(kFixturePath));
    const int test_pid = 1234;
    std::string filepath = kFixturePath + LinuxParser::kProcDirectory + std::to_string(test_pid) + LinuxParser::kStatFilename;
    std::string line;
    std::ifstream stream(filepath);

    if (stream.is_open())
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> proc_stat;
    }

    EXPECT_EQ(117, proc_stat.utime());
    EXPECT_EQ(333, proc_stat.stime());
    EXPECT_EQ(1471, proc_stat.cutime());
    EXPECT_EQ(1586, proc_stat.cstime());
    EXPECT_EQ(3507, proc_stat.total_time());
}