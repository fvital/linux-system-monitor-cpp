#include <cctype>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>

#include "linux_parser.h"
#include "process.h"

using std::string;

int Process::Pid() const { return pid_; }
int Process::Uid() const { return uid_; }

float Process::CpuUtilization() const
{
    return proc_stat_.utilization();
}

string Process::Command() const
{
    std::string line;
    std::string filepath = root_ + LinuxParser::kProcDirectory + std::to_string(Pid()) + LinuxParser::kCmdlineFilename;
    std::ifstream stream(filepath);

    if (stream.is_open())
    {
        std::getline(stream, line);
    }
    return line;
}

string Process::Ram(int precision) const
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << vm_size_ / 1024.0;
    return stream.str();
}

string Process::User() const { return user_; }

double Process::UpTime() const
{
    return proc_stat_.uptime(true);
}

bool Process::operator<(Process const &a) const
{
    return CpuUtilization() > a.CpuUtilization();
}

Process::Process(int pid, const std::map<int, std::string> &user_map, double sys_uptime, const std::string &root) : pid_{pid}, root_{root}
{
    parse_status(root_);
    update_data(user_map, sys_uptime, root_);
};

Process &Process::parse_status(const std::string &root)
{

    std::string name;
    std::string line;
    std::string filepath = root + LinuxParser::kProcDirectory + std::to_string(Pid()) + LinuxParser::kStatusFilename;
    std::ifstream stream(filepath);

    if (stream.is_open())
    {
        while (std::getline(stream, line))
        {
            std::istringstream linestream(line);
            std::getline(linestream, name, ':');

            if (name == "State")
            {
                linestream >> state_;
            }
            else if (name == "Uid")
            {
                linestream >> uid_;
            }
            else if (name == "VmSize")
            {
                linestream >> vm_size_;

                /*
                Assumes the checks have been written in
                the order they appear in the file,
                and we have no interest in the information
                after this point.
                Move 'break' call as needed if more cases are included.
                */
                break;
            }
            else
            {
                continue;
            }
        }
    }
    return *this;
}

Process &Process::update_data(const std::map<int, std::string> &user_map, double sys_uptime, const std::string &root)
{
    auto user_it = user_map.find(uid_);
    if (user_it == user_map.end())
    {
        user_ = "_unknown_";
    }
    else
    {
        user_ = user_it->second;
    }
    proc_stat_.set_sys_uptime(sys_uptime);
    std::string stat_filepath = root + LinuxParser::kProcDirectory + std::to_string(pid_) + LinuxParser::kStatFilename;
    std::ifstream statstream(stat_filepath);
    std::string line;
    if (statstream.is_open())
    {
        std::getline(statstream, line);
        std::istringstream linestream(line);
        linestream >> proc_stat_;
    }

    return *this;
}