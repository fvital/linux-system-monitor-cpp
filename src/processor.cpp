#include <string>
#include <fstream>
#include <sstream>

#include "linux_parser.h"
#include "processor.h"

float Processor::Utilization() const
{
    return agg_cpu_.utilization();
}

Processor &Processor::update_stat(const std::string &root)
{
    std::string name;
    std::string line;
    std::ifstream stream(root + LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

    if (stream.is_open())
    {
        while (std::getline(stream, line))
        {
            std::istringstream linestream(line);
            linestream >> name;
            if (name == "cpu")
            {
                linestream >> agg_cpu_;
            }
            else if (name.find("cpu") == 0)
            {
                int idx = std::stoi(name.substr(3));
                if (cpus_.size() <= idx)
                {
                    cpus_.emplace_back(CpuStat());
                }
                linestream >> cpus_[idx];
            }
            else if (name == "processes")
            {
                linestream >> total_cumulative_processes_;
            }
            else if (name == "procs_running")
            {
                linestream >> procs_running_;
            }
            else
            {
                continue;
            }
        }
    }
    return *this;
}

const CpuStat &Processor::get_cpu() const
{
    return agg_cpu_;
}

const CpuStat &Processor::get_cpu(size_t idx) const
{
    return cpus_.at(idx);
}

int Processor::get_cpu_count() const
{
    return cpus_.size();
}

int Processor::get_procs_running() const
{
    return procs_running_;
}
int Processor::get_total_cumulative_processes() const
{
    return total_cumulative_processes_;
}