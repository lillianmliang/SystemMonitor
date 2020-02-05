#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include <unistd.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Helper function to use when format of desired information in file is "keyname value"
string getValue(string filename, string keyname){
  string line;
  string key;
  string value;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyname) {
          return value;
        }
      }
    }
  }
  return "";
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  float memtotal = stof(getValue(kProcDirectory + kMeminfoFilename, "MemTotal:"));
  float memfree = stof(getValue(kProcDirectory + kMeminfoFilename, "MemFree:"));
  return (memtotal-memfree)/memtotal;
}

long LinuxParser::UpTime() { 
  string s_uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> s_uptime;
    return stol(s_uptime);
  }
  return 0; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> CPUUtil(10);
  string cpu;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> 
    CPUUtil[kUser_]>>
    CPUUtil[kNice_]>>
    CPUUtil[kSystem_]>>
    CPUUtil[kIdle_]>>
    CPUUtil[kIOwait_]>>
    CPUUtil[kIRQ_]>>
    CPUUtil[kSoftIRQ_]>>
    CPUUtil[kSteal_]>>
    CPUUtil[kGuest_]>>
    CPUUtil[kGuestNice_];
  }
  return CPUUtil; 
}

int LinuxParser::TotalProcesses() { return stoi(getValue(kProcDirectory+kStatFilename, "processes"));}

int LinuxParser::RunningProcesses() {return stoi(getValue(kProcDirectory + kStatFilename, "procs_running"));}

string LinuxParser::Command(int pid) { 
  string command;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid)+kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
    return command;
  }
  return string(); 
}

string LinuxParser::Ram(int pid) { 
  string ram_value = getValue(kProcDirectory+to_string(pid)+kStatusFilename, "VmSize:"); 
  if (ram_value == "")
    return "0";
  
  int mem_kB = stoi(ram_value);
  int mem_MB = mem_kB/1000;
  return to_string(mem_MB);
}

string LinuxParser::Uid(int pid) { return getValue(kProcDirectory+to_string(pid)+kStatusFilename, "Uid:"); }

string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line;
  string user;
  string x;
  string value;
  std::ifstream filestream("/etc/passwd");
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x>>value) {
        if (value == uid) {
          return user;
        }
      }
    }
  }
  return string(); 
}

long LinuxParser::UpTime(int pid) { 
  string sutime;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid)+kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i<22; i++){
      linestream >> sutime;
    }
    return UpTime() - stol(sutime)/sysconf(_SC_CLK_TCK);
  }
  return 0;
}

float LinuxParser::CpuUtilization(int pid) {
  int utime; //14
  int stime; //15
  int cutime; //16
  int cstime; //17
  string temp;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid)+kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i<=17; i++){
      linestream >> temp;
      if (i == 14) utime = stoi(temp);
      if (i == 15) stime = stoi(temp);
      if (i == 16) cutime = stoi(temp);
      if (i == 17) cstime = stoi(temp);
    }
  }
  float total_time = (float)(utime+stime+cutime+cstime)/(float)sysconf(_SC_CLK_TCK);
  return total_time/(float)UpTime(pid);
}