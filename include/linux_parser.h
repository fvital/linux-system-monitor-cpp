#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser
{
  // Paths
  const std::string kProcDirectory{"/proc/"};
  const std::string kCmdlineFilename{"/cmdline"};
  const std::string kCpuinfoFilename{"/cpuinfo"};
  const std::string kStatusFilename{"/status"};
  const std::string kStatFilename{"/stat"};
  const std::string kUptimeFilename{"/uptime"};
  const std::string kMeminfoFilename{"/meminfo"};
  const std::string kVersionFilename{"/version"};
  const std::string kOSPath{"/etc/os-release"};
  const std::string kPasswordPath{"/etc/passwd"};

  // System
  [[deprecated("Use MemInfo from 'meminfo.h' to retrieve, store and access this info instead.")]] float MemoryUtilization(const std::string &root = "/");
  float UpTime(const std::string &root = "/");
  std::vector<int> Pids(const std::string &root = "/");
  int TotalProcesses(const std::string &root = "/");
  int RunningProcesses();
  std::string OperatingSystem(const std::string &root = "/");
  std::string Kernel(const std::string &root = "/");

  // CPU
  enum CPUStates
  {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
  };
  std::vector<std::string> CpuUtilization(const std::string &root = "/");
  long Jiffies(const std::string &root = "/");
  long ActiveJiffies(const std::string &root = "/");
  long ActiveJiffies(int pid);
  long IdleJiffies(const std::string &root = "/");

  // Processes
  std::string Command(int pid);
  std::string Ram(int pid);
  std::string Uid(int pid);
  std::string User(int pid);
  long int UpTime(int pid);
}; // namespace LinuxParser

#endif