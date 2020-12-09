#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>
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

// DONR: Read and return the system memory utilization

float LinuxParser::MemoryUtilization() { 
  float memTotal, memAvailable, value;
  string line{""};
  string key{""};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = value;
        }
        else if (key == "MemAvailable:") {
          memAvailable = value;
        }
      }
    }
  }
  return (memTotal - memAvailable) / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long systemUpTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> systemUpTime;
    return systemUpTime;
  }
  return 0;
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string key;
  long value;
  string line;
  long jiffiesCount{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> key){
      if(key == "cpu"){
        while(linestream >> value){
          jiffiesCount += value;
        }
        stream.close();
        return jiffiesCount;
      }
    }
  }
  return -1;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string valuesToIgnore{""};
  long utime, stime, cutime, cstime;
  long activeJiffiesForPID{};
  string pidDirectory = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pidDirectory + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 13; i++){
      linestream >> valuesToIgnore;
    }
    linestream >> utime >> stime >> cutime >> cstime;
    activeJiffiesForPID = utime + stime + cutime + cstime;
    return activeJiffiesForPID; 
  }
  return -1;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  long cpu, user, nice, system, idle, iowait, irq;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq;
    
    return idle + iowait; 
  }
  return -1; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string key;
  vector<string> cpuUtilisation{};
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key; 
      if (key == "cpu") {
         while(!linestream.eof()){
           string util;
           linestream >> util;
           cpuUtilisation.push_back(util);
         }
        return cpuUtilisation;
      }
    }
  }
  return {}; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string pidDirectory = std::to_string(pid);
  string key;
  float value;
  string line;
  std::ifstream stream(kProcDirectory + pidDirectory + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          std::ostringstream result;
          result << std::setprecision(6) << value / 1000;
          return result.str();
        }
      }
    }
  }
  return string();  
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string pidDirectory = std::to_string(pid);
  string key;
  string value;
  string line;
  std::ifstream stream(kProcDirectory + pidDirectory + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return string();   
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string userId = LinuxParser::Uid(pid);

  string username, xToIgnore, id;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> xToIgnore >> id;
      if (id == userId) {
        return username;
      }   
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line{""};
  string valuesToIgnore{""};
  long processUpTime{};
  string pidDirectory = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pidDirectory + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 21; i++){
      linestream >> valuesToIgnore;
    }
    linestream >> processUpTime;
    return processUpTime / LinuxParser::systemClock; 
  }
  return 1;
}