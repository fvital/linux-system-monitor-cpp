#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem(const std::string &root)
{
  string line;
  string key;
  string value;
  std::ifstream filestream(root + kOSPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "PRETTY_NAME")
        {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel(const std::string &root)
{
  string os, kernel, version;
  string line;
  std::ifstream stream(root + kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids(const std::string &root)
{
  vector<int> pids;
  DIR *directory = opendir((root + kProcDirectory).c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr)
  {
    // Is this a directory?
    if (file->d_type == DT_DIR)
    {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit))
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization(const std::string &root)
{
  return 0;
}

double LinuxParser::UpTime(const std::string &root)
{
  double uptime, idle_time;
  string line;
  std::ifstream stream(root + kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle_time;
  }
  return uptime;
}

long LinuxParser::Jiffies(const std::string &root)
{
  string name;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(root + kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  return user + nice + system + idle + iowait + irq + softirq + steal;
}

long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

long LinuxParser::ActiveJiffies(const std::string &root)
{
  string name;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(root + kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  return user + nice + system + irq + softirq + steal;
}

long LinuxParser::IdleJiffies(const std::string &root)
{
  string name;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(root + kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  return idle + iowait;
}

vector<string> LinuxParser::CpuUtilization(const std::string &root)
{
  vector<string> out{};
  out.push_back(std::to_string(static_cast<float>(LinuxParser::ActiveJiffies(root)) / static_cast<float>(LinuxParser::Jiffies(root))));
  return out;
}

int LinuxParser::TotalProcesses(const std::string &root)
{
  return LinuxParser::Pids(root).size();
}

int LinuxParser::RunningProcesses() { return 0; }

string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
