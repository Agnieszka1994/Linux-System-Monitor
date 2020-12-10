#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

void Process::updateID(int newId){
    pid_ = newId;
}

int Process::Pid() { 
    return pid_; 
}

float Process::CpuUtilization() const { 

    const float total_time = LinuxParser::ActiveJiffies(pid_);
    const float systemUpTime = LinuxParser::UpTime();
    const float processStartTime = LinuxParser::UpTime(pid_); 
    const float processDurationInSeconds = systemUpTime - processStartTime;
    
    return  (total_time / LinuxParser::systemClock) / processDurationInSeconds;
}

string Process::Command() { 
    return LinuxParser::Command(pid_); 
}

string Process::Ram() { 
    return LinuxParser::Ram(pid_);
 }

string Process::User() { 
    return LinuxParser::User(pid_); 
}

long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_); 
}

bool Process::operator<(const Process& other) const{ 
    return CpuUtilization()  < other.CpuUtilization(); 
}

bool Process::operator>(const Process& other) const{ 
    return CpuUtilization()  > other.CpuUtilization(); 
}