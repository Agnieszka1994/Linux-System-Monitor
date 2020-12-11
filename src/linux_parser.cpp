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

string LinuxParser::OperatingSystem() {
  string key{""}, line{""}, value{""};
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
          filestream.close();
          return value;
        }
      }
    }
    filestream.close();
  }
  return value;
}

string LinuxParser::Kernel() {
  string os{""}, version{""}, kernel{""}, line{""};
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;

    stream.close();
    return kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids{};
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    
    if (file->d_type == DT_DIR) {
      
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
  float memTotal{1}, memAvailable{}, value{};
  string line{""}, key{""};
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
    stream.close();
    return (memTotal - memAvailable) / memTotal;
  }
  return 1;
}

long LinuxParser::UpTime() { 
  long systemUpTime{};
  string line{""};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> systemUpTime;
    stream.close();
    return systemUpTime;
  }
  return 0;
  }


long LinuxParser::Jiffies() { 
  string key{""}, line{""};
  long value{}, jiffiesCount{};
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

long LinuxParser::ActiveJiffies(int pid) { 
  string line{""}, valuesToIgnore{""};
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
    stream.close();
    return activeJiffiesForPID; 
  }
  return -1;
}

long LinuxParser::ActiveJiffies() { 
  return Jiffies() - IdleJiffies (); 
}

long LinuxParser::IdleJiffies() { 
  string line{""};
  long cpu{}, user{}, nice{}, system{}, idle{}, iowait{}, irq{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq;
    stream.close();
    return idle + iowait; 
  }
  return -1; 
}

vector<string> LinuxParser::CpuUtilization() { 
  string key{""},line{""};
  vector<string> cpuUtilisation{};
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
        stream.close();
        return cpuUtilisation;
      }
    }
  }
  return {}; 
}

int LinuxParser::TotalProcesses() { 
  string key{""}, line{""};
  int value{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          stream.close();
          return value;
        }
      }
    }
    stream.close();
  }
  return 0;
}

int LinuxParser::RunningProcesses() { 
  string key{""}, line{""};
  int value{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          stream.close();
          return value;
        }
      }
    }
    stream.close();
  }
  return 0; 
}

string LinuxParser::Command(int pid) { 
  string line;
  string pidDirectory = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pidDirectory + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string();
}

string LinuxParser::Ram(int pid) { 
  string pidDirectory = std::to_string(pid);
  string key{""}, line{""};
  float value{};
  std::ifstream stream(kProcDirectory + pidDirectory + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          std::ostringstream result;
          result << std::setprecision(6) << value / 1000;
          stream.close();
          return result.str();
        }
      }
    }
    stream.close();
  }
  return string();  
}

string LinuxParser::Uid(int pid) { 
  string pidDirectory = std::to_string(pid);
  string key{""}, line{""}, value{""};
  std::ifstream stream(kProcDirectory + pidDirectory + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          stream.close();
          return value;
        }
      }
    }
    stream.close();
  }
  return value;   
}

string LinuxParser::User(int pid) { 
  string userId = LinuxParser::Uid(pid);
  string username{""}, xToIgnore{""}, id{""}, line{""};
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> xToIgnore >> id;
      if (id == userId) {
        stream.close();
        return username;
      }   
    }
    stream.close();
  }
  return string(); 
}

long LinuxParser::UpTime(int pid) { 
  string line{""}, valuesToIgnore{""};
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
    stream.close();
    return processUpTime / LinuxParser::systemClock; 
  }
  return 1;
}