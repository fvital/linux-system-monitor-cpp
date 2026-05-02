#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

#include "stat.h"
class Processor
{
public:
    /// Returns aggregate CPU utilization
    float Utilization() const;

    /// @param root Optional path to be used as root directory. Intended for testing.
    Processor &update_stat(const std::string &root = "/");

    /// @param idx If provided, returns CpuStat object with data of the individual cpu with that index.
    ///            If omitted, returns CpuStat object with aggregate cpu data.
    const CpuStat &get_cpu() const;
    const CpuStat &get_cpu(size_t idx) const;
    int get_cpu_count() const;
    int get_procs_running() const;
    int get_total_cumulative_processes() const;

private:
    CpuStat agg_cpu_;
    std::vector<CpuStat> cpus_;
    int procs_running_ = -1;
    int total_cumulative_processes_ = -1; ///< Number of forks since boot.
};

#endif // PROCESSOR_H