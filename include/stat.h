#ifndef STAT_H
#define STAT_H

#include <array>
#include <sstream>
#include <string>
#include <unistd.h>

#include "linux_parser.h"

/// Parsed representation of a single cpu* line from /proc/stat.
/// Field order matches the kernel ABI: user nice system idle iowait irq softirq steal guest guest_nice.
class CpuStat
{
public:
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice; ///< Measured in jiffies.
    CpuStat() = default;
    CpuStat(long user, long nice,
            long system, long idle,
            long iowait, long irq,
            long softirq, long steal,
            long guest, long guest_nice);
    long total() const;  ///< Sum of all time fields (excludes guest, which is already in user).
    long active() const; ///< Total minus idle and iowait.
    float utilization() const;
};

/// @param iss Expects a stringstream pointing to a cpu* line from /proc/stat without the first column (name).
std::istringstream &operator>>(std::istringstream &iss, CpuStat &cpustat);

/// Parsed representation of /proc/[pid]/stat.
/// Field indices follow the kernel ABI documented in proc(5).
class ProcessStat
{
    using stat_datatype = std::array<std::string, 52>;
    stat_datatype data_ = {};
    long query_time_elem(size_t idx, bool in_secs = false) const;
    double sys_uptime_ = 0;

public:
    /// @param in_secs  If true, convert the result from clock ticks to seconds.
    ///                 If false, return the raw clock ticks count.
    long utime(bool in_secs = false) const;
    long stime(bool in_secs = false) const;
    long cutime(bool in_secs = false) const;
    long cstime(bool in_secs = false) const;
    long total_time(bool in_secs = false) const;
    long starttime(bool in_secs = false) const;

    double uptime(bool in_secs = true) const;

    /// CPU utilization as a fraction of process uptime.
    double utilization() const;

    friend std::istringstream &operator>>(std::istringstream &iss, ProcessStat &procstat);
    ProcessStat() = default;
    ProcessStat(stat_datatype &&data);

    /// @param iss Stringstream pointing to the first line of /proc/[pid]/stat.
    ProcessStat(std::istringstream &iss);

    ProcessStat &set_sys_uptime(double v);
    double get_sys_uptime() const;
};

/// @param iss Stringstream pointing to the first line of /proc/[pid]/stat.
std::istringstream &operator>>(std::istringstream &iss, ProcessStat &procstat);

#endif // STAT_H