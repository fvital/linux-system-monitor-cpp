
#include <gtest/gtest.h>

#include "processor.h"

const std::string kFixturePath = std::string(TEST_FIXTURE_DIR); // injected via CMake

TEST(ProcessorT, aggCpu)
{
    Processor p1;
    p1.update_stat(kFixturePath);

    EXPECT_EQ(1386404, p1.get_cpu().user);
    EXPECT_EQ(16016, p1.get_cpu().nice);
    EXPECT_EQ(1089961, p1.get_cpu().system);
    EXPECT_EQ(6, p1.get_cpu_count());
}

TEST(ProcessorT, singleCpu0)
{
    Processor p1;
    p1.update_stat(kFixturePath);

    EXPECT_EQ(232444, p1.get_cpu(0).user);
    EXPECT_EQ(2607, p1.get_cpu(0).nice);
    EXPECT_EQ(221639, p1.get_cpu(0).system);
}

TEST(ProcessorT, singleCpu1)
{
    Processor p1;
    p1.update_stat(kFixturePath);

    EXPECT_EQ(315113, p1.get_cpu(1).user);
    EXPECT_EQ(1764, p1.get_cpu(1).nice);
    EXPECT_EQ(214142, p1.get_cpu(1).system);
}

TEST(ProcessorT, Utilization)
{
    Processor p1;
    p1.update_stat(kFixturePath);

    EXPECT_FLOAT_EQ(0.01108854092, p1.Utilization());
}

TEST(ProcessorT, procs_running)
{
    Processor p1;
    p1.update_stat(kFixturePath);

    EXPECT_EQ(1, p1.get_procs_running());
}

TEST(ProcessorT, total_cumulative_processes)
{
    Processor p1;
    p1.update_stat(kFixturePath);

    EXPECT_EQ(83061, p1.get_total_cumulative_processes());
}