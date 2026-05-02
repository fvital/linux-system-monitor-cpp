#ifndef SYSTEM_H
#define SYSTEM_H

#include <map>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System
{
public:
  Processor &Cpu();
  std::vector<Process> &Processes();
  float MemoryUtilization();
  long UpTime() const;
  int TotalProcesses() const;
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();
  System(const std::string &root = "/");
  std::string User(int uid) const;

private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string operating_system_ = "";
  std::string kernel_ = "";
  std::map<int, std::string> user_map_;
  System &update_user_map(const std::string &root = "/");
  System &clean_process_list(const std::string &root = "/");
  System &update_process_list(const std::string &root = "/");
  const std::string root_ = "/";
};

#endif // SYSTEM_H