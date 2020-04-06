#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_'); // replaces spaces to make it easier to parse. 
      std::replace(line.begin(), line.end(), '=', ' '); // makes it easy to pop off as tokens 
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
    std::istringstream linestream{line}; // Creating a string stream lets us pull tokens from the string. 
    linestream >> os;
    linestream >> version;
    linestream >> kernel;
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

// use MemTotal - MemFree from /proc/meminfo
float LinuxParser::MemoryUtilization() {
  float memtotal, memfree, val;
  string line, key;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream{line};
      linestream >> key >> val;
      if (key == "MemTotal:"){
        memtotal = val;
      } else if (key == "MemFree:"){
        memfree = val;
      };
    };
  }
  return (memtotal - memfree) / memtotal; 
}

long LinuxParser::UpTime() { 
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream{line};
    linestream >> uptime;
  }
  return uptime;
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

// TODO: Read and return CPU utilization
vector<int> LinuxParser::CpuUtilization() { 
  string line, cpu;
  int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream{line};
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }

  return {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice}; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key;
  int val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream{line};
      linestream >> key >> val;
      if (key == "processes"){
        return val;
      }
    }
  }
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key;
  int val, running;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream{line};
      linestream >> key >> val;
      if (key == "procs_running"){
        running = val;
      }
    }
  }
  return running;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string com, line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream{line};
    linestream >> com;
  } 
  return com; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, val;
  int vmsize;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream{line};
      linestream >> key >> val;
      if (key == "VmSize:"){
        vmsize = std::stoi(val);
      }
    }
  }
  int mb = (vmsize / 1000);
  return to_string(mb); 
}

float LinuxParser::CpuUtilization(int pid){
  string line, val;
  long utime, stime, cutime, cstime, starttime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
  std::getline(stream, line);
  std::istringstream linestream{line};
  for (int i = 1; i <= 22; i++){
    linestream >> val;
    if (i == 14){
      utime = std::stol(val);
    }
    if (i == 15){
      stime = std::stol(val);
    }
    if (i == 16){
      cutime = std::stol(val);
    }
    if (i == 17){
      cstime = std::stol(val);
    }
    if (i == 22){
      starttime = std::stol(val);
    }
  }
  long total_time = utime + stime + cutime + cstime;
  float seconds = UpTime() - (starttime / sysconf(_SC_CLK_TCK));
  return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
  }
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string key, line, uid, val;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream{line};
      linestream >> key >> val;
      if (key == "Uid:"){
        uid = val;
      }
    }
  }
  return uid; 
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string seekingUid = Uid(pid); 
  string key, line, name, pw, uid, username;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream{line};
      linestream >> name >> pw >> uid;
      if (uid == seekingUid){
        username = name;
      }
    }
  }
  return username;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, val;
  long starttime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream{line};
    for (int i = 1; i <= 22; i++){
      linestream >> val;
      if (i == 22){
        starttime = std::stol(val);
      }
    }
  }
  return UpTime() - (starttime / sysconf(_SC_CLK_TCK));
}