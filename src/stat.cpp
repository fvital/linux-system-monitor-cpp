#include <sstream>
#include <string>
#include <unistd.h>

#include "linux_parser.h"
#include "stat.h"

CpuStat::CpuStat(
    long user, long nice, long system,
    long idle, long iowait, long irq,
    long softirq, long steal, long guest,
    long guest_nice) : user{user}, nice{nice}, system{system},
                       idle{idle}, iowait{iowait}, irq{irq},
                       softirq{softirq}, steal{steal}, guest{guest},
                       guest_nice{guest_nice} {};

long CpuStat::total() const
{
    return user + nice + system + idle + iowait + irq + softirq + steal;
}
long CpuStat::active() const
{
    return user + nice + system + irq + softirq + steal;
}

float CpuStat::utilization() const
{
    return static_cast<float>(active()) / static_cast<float>(total());
}

std::istringstream &operator>>(std::istringstream &iss, CpuStat &cpustat)
{
    iss >> cpustat.user >> cpustat.nice;
    iss >> cpustat.system >> cpustat.idle;
    iss >> cpustat.iowait >> cpustat.irq;
    iss >> cpustat.softirq >> cpustat.steal;
    iss >> cpustat.guest >> cpustat.guest_nice;

    return iss;
}

/// Indices of /proc/[pid]/stat fields.
/// Indices not used by this application were omitted.
enum class ProcPidStatIdx : size_t
{
    utime = 13,
    stime = 14,
    cutime = 15,
    cstime = 16,
    starttime = 21
};

long ProcessStat::query_time_elem(size_t idx, bool in_secs) const
{
    // TODO: Protect against using indices that point to string or other incompatible types.
    // starttime is documented as long long, so it might need to be treated separately.
    if (in_secs)
    {
        return std::stol(data_[idx]) / sysconf(_SC_CLK_TCK);
    }
    else
    {
        return std::stol(data_[idx]);
    }
}

long ProcessStat::utime(bool in_secs) const
{
    return query_time_elem(static_cast<size_t>(ProcPidStatIdx::utime), in_secs);
}

long ProcessStat::stime(bool in_secs) const
{
    return query_time_elem(static_cast<size_t>(ProcPidStatIdx::stime), in_secs);
}

long ProcessStat::cutime(bool in_secs) const
{
    return query_time_elem(static_cast<size_t>(ProcPidStatIdx::cutime), in_secs);
}

long ProcessStat::cstime(bool in_secs) const
{
    return query_time_elem(static_cast<size_t>(ProcPidStatIdx::cstime), in_secs);
}

long ProcessStat::total_time(bool in_secs) const
{
    /// sum before converting to seconds to reduce impact of rounding.
    long tmp = utime(false) + stime(false) + cutime(false) + cstime(false);
    if (in_secs)
    {
        return tmp / sysconf(_SC_CLK_TCK);
    }
    else
    {
        return tmp;
    }
}

long ProcessStat::starttime(bool in_secs) const
{
    return query_time_elem(static_cast<size_t>(ProcPidStatIdx::starttime), in_secs);
}

double ProcessStat::uptime(bool in_secs, const std::string &root) const
{
    if (in_secs)
    {
        return LinuxParser::UpTime(root) - static_cast<double>(starttime(false)) / static_cast<double>(sysconf(_SC_CLK_TCK));
    }
    else
    {
        return static_cast<double>(LinuxParser::UpTime(root)) * static_cast<double>(sysconf(_SC_CLK_TCK)) - static_cast<double>(starttime(false));
    }
}

double ProcessStat::utilization(const std::string &root) const
{
    return static_cast<double>(total_time(false)) / uptime(false, root);
}

std::istringstream &operator>>(std::istringstream &iss, ProcessStat &procstat)
{
    for (size_t i = 0; i < 52; ++i)
    {
        iss >> procstat.data_[i];
    }
    return iss;
};

ProcessStat::ProcessStat(std::istringstream &iss)
{
    iss >> *this;
}

ProcessStat::ProcessStat(stat_datatype &&data) : data_{std::move(data)} {};
