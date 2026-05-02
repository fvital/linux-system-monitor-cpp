#ifndef PROCESS_H
#define PROCESS_H

#include <map>
#include <string>

#include "stat.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process
{
public:
    int Pid() const;
    int Uid() const;
    std::string User() const;
    std::string Command() const;
    float CpuUtilization() const;
    /// Memory utilization in MB
    /// @param precision Number of digits to include after decimal point.
    std::string Ram(int precision = 2) const;
    double UpTime() const;

    /// Sort by descending CPU utilization.
    bool operator<(Process const &a) const;
    Process(int pid, const std::map<int, std::string> &user_map, double sys_uptime, const std::string &root = "/");
    Process &update_data(const std::map<int, std::string> &user_map, double sys_uptime, const std::string &root = "/");

private:
    int pid_;
    int uid_;
    int vm_size_;
    std::string state_;
    std::string root_;
    std::string user_;
    ProcessStat proc_stat_;

    Process &parse_status(const std::string &root = "/");
};

#endif