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
    int Uid() const;              // TODO: See src/process.cpp
    std::string User() const;     // TODO: See src/process.cpp
    std::string Command() const;  // TODO: See src/process.cpp
    float CpuUtilization() const; // TODO: See src/process.cpp
    /// Memory utilization in MB
    /// @param precision Number of digits to include after decimal point.
    std::string Ram(int precision = 2) const; // TODO: See src/process.cpp
    long int UpTime() const;                  // TODO: See src/process.cpp

    /// Sort by descending CPU utilization.
    bool operator<(Process const &a) const; // TODO: See src/process.cpp
    Process(int pid, const std::map<int, std::string> &user_map, const std::string &root = "/");
    Process &update_data(const std::map<int, std::string> &user_map, const std::string &root = "/");

    // TODO: Declare any necessary private members
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