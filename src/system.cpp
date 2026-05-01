#include <algorithm>
#include <cmath>
#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <fstream>

#include "linux_parser.h"
#include "meminfo.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor &System::Cpu()
{
    cpu_.update_stat();
    return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process> &System::Processes()
{
    clean_process_list(root_).update_process_list(root_);
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel()
{
    if (!kernel_.empty())
        return kernel_;
    kernel_ = LinuxParser::Kernel(root_);
    return kernel_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization()
{
    std::ifstream ifs{root_ +
                      LinuxParser::kProcDirectory +
                      LinuxParser::kMeminfoFilename};
    MemInfo m1{ifs};

    return (m1.mem_total - m1.mem_free) / m1.mem_total;
}

// TODO: Return the operating system name
std::string System::OperatingSystem()
{
    if (!operating_system_.empty())
        return operating_system_;
    operating_system_ = LinuxParser::OperatingSystem(root_);
    return operating_system_;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(root_); }

// TODO: Return the number of seconds since the system started running
long System::UpTime()
{
    return static_cast<long>(std::round(LinuxParser::UpTime(root_)));
}
System &System::update_user_map(const std::string &root)
{
    std::string name;
    std::string tmp;
    std::string uid;
    std::string line;
    std::ifstream stream(root + LinuxParser::kPasswordPath);

    if (stream.is_open())
    {
        while (std::getline(stream, line))
        {
            std::istringstream linestream(line);
            std::getline(linestream, name, ':');
            std::getline(linestream, tmp, ':');
            std::getline(linestream, uid, ':');

            user_map_[std::stoi(uid)] = name;
        }
    }
    return *this;
}

System &System::clean_process_list(const std::string &root)
{
    auto pid_list = LinuxParser::Pids(root);
    for (auto it = processes_.begin(); it != processes_.end();)
    {
        if (std::find(pid_list.begin(), pid_list.end(), (*it).Pid()) == pid_list.end())
        {
            it = processes_.erase(it);
        }
        else
        {
            ++it;
        }
    }
    // std::sort(processes_.begin(), processes_.end());
    return *this;
}

System &System::update_process_list(const std::string &root)
{
    auto pid_list = LinuxParser::Pids(root);

    for (auto pid : pid_list)
    {
        auto proc_it = std::find_if(processes_.begin(), processes_.end(), [pid](Process &proc)
                                    { return proc.Pid() == pid; });
        if (proc_it == processes_.end())
        {
            processes_.emplace_back(Process(pid, user_map_, root));
        }
        else
        {
            proc_it->update_data(user_map_, root);
        }
    }

    std::sort(processes_.begin(), processes_.end());
    return *this;
}

std::string System::User(int uid) const
{
    // TODO: Deal with case of uid not present in user_map_.
    return user_map_.at(uid);
}

System::System(const std::string &root) : root_{root}
{
    update_user_map(root_);
    clean_process_list(root_);
    update_process_list(root_);
}
