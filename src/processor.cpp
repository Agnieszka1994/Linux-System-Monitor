#include "processor.h"
#include "linux_parser.h"

using LinuxParser::kStatFilename;
using LinuxParser::kProcDirectory;
using std::string;

float Processor::Utilization() { 

  float user, nice, system, idle, iowait, irq;
 
  string line{""};
  string key{""};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
      std::istringstream linestream(line);
      std::string key{};
      linestream >> key >> user >> nice >> system >> idle >> iowait >> irq;
  }

    const float Idle = idle + iowait;
    const float NonIdle = user + nice + system + irq;

    const float PrevTotal = idle_ + nonIdle_;
    const float Total = Idle + NonIdle;
    
    const float TotalDelta = Total - PrevTotal;
    const float IdleDelta = Idle - idle_;

    const float CPU_Percentage = (TotalDelta - IdleDelta) / TotalDelta;

    idle_ = Idle;
    nonIdle_ = NonIdle;
    
    return CPU_Percentage;
}