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
  Processor &Cpu();                  // TODO: See src/system.cpp
  std::vector<Process> &Processes(); // TODO: See src/system.cpp
  float MemoryUtilization();         // TODO: See src/system.cpp
  long UpTime();                     // TODO: See src/system.cpp
  int TotalProcesses();              // TODO: See src/system.cpp
  int RunningProcesses();            // TODO: See src/system.cpp
  std::string Kernel();              // TODO: See src/system.cpp
  std::string OperatingSystem();     // TODO: See src/system.cpp
  System(const std::string &root = "/");
  std::string User(int uid) const;
  // TODO: Define any necessary private members
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

#endif