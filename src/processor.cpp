#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

// calculates utilization using algorithm given in https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
  vector<string> CPUUtil = LinuxParser::CpuUtilization();
  float user =  stof(CPUUtil[LinuxParser::kUser_])*User_Hz;
  float nice = stof( CPUUtil[LinuxParser::kNice_])*User_Hz;
  float systemtime = stof(CPUUtil[LinuxParser::kSystem_])*User_Hz;
  float idle = stof(CPUUtil[LinuxParser::kIdle_])*User_Hz;
  float iowait = stof(CPUUtil[LinuxParser::kIOwait_])*User_Hz;
  float irq = stof(CPUUtil[LinuxParser::kIRQ_])*User_Hz;
  float softirq = stof(CPUUtil[LinuxParser::kSoftIRQ_])*User_Hz;
  float steal = stof(CPUUtil[LinuxParser::kSteal_])*User_Hz;

    float Idle = idle +iowait;
    float NonIdle = user + nice + systemtime + irq + softirq + steal;
    float Total = Idle + NonIdle;

    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;

    float CPUUtilization = (totald-idled)/totald;
    PrevTotal = Total;
    PrevIdle = Idle;
    return CPUUtilization; 
}