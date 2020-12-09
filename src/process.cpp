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

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    // CLOCK TO BE CORRECTED    
    //auto clock{ sysconf(_SC_CLK_TCK) };
    float total_time = LinuxParser::ActiveJiffies(pid_);
    float systemUpTime = LinuxParser::UpTime();
    float processStartTime = LinuxParser::UpTime(pid_); 
    float processDurationInSeconds = systemUpTime - processStartTime;
    
    
    return  (total_time / LinuxParser::systemClock) / processDurationInSeconds;
 
}



// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_);
 }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(const Process& other) const{ 
    return CpuUtilization()  < other.CpuUtilization(); 
}

bool Process::operator>(const Process& other) const{ 
    return CpuUtilization()  > other.CpuUtilization(); 
}