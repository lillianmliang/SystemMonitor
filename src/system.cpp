#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include <algorithm>

using std::set;
using std::size_t;
using std::string;
using std::vector;

string OS{""};
string kernel{""};

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
    processes_.clear();
    for (int pid:LinuxParser::Pids()){
        Process process;
        process.Pid(pid);
        processes_.push_back(process);
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

std::string System::Kernel() { 
    if (kernel=="") kernel = LinuxParser::Kernel();
    return kernel;  
}

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { 
    if (OS=="") OS = LinuxParser::OperatingSystem();
    return OS; 
}  

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() {return LinuxParser::UpTime(); }