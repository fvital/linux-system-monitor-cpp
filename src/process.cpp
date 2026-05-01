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

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }
int Process::Uid() const { return uid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const
{
    return proc_stat_.utilization(root_);
}

// TODO: Return the command that generated this process
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

// TODO: Return this process's memory utilization
string Process::Ram(int precision) const
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << vm_size_ / 1024.0;
    return stream.str();
}

// TODO: Return the user (name) that generated this process
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds)
long Process::UpTime() const
{
    return proc_stat_.uptime(true, root_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const
{
    return CpuUtilization() > a.CpuUtilization();
}

Process::Process(int pid, const std::map<int, std::string> &user_map, const std::string &root) : pid_{pid}, root_{root}
{
    parse_status(root_);
    update_data(user_map, root_);
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

Process &Process::update_data(const std::map<int, std::string> &user_map, const std::string &root)
{
    // TODO: look into/handle case of wrong/missing uid.
    user_ = user_map.at(uid_);

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