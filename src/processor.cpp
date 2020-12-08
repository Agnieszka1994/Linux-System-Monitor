#include "processor.h"
#include "linux_parser.h"

using LinuxParser::kStatFilename;
using LinuxParser::kProcDirectory;
using std::string;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  float user, nice, system, idle, iowait, irq;
  //static float prevUser, prevNice, prevSystem, prevIdle, prevIowait, prevIrq;
  string line{""};
  string key{""};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
      std::istringstream linestream(line);
      std::string key{};
      linestream >> key >> user >> nice >> system >> idle >> iowait >> irq;
  }
    static float PrevIdle{};
    static float PrevNonIdle{};

    float Idle = idle + iowait;

    float NonIdle = user + nice + system + irq;

    float PrevTotal = PrevIdle + PrevNonIdle;
    float Total = Idle + NonIdle;
    
    float TotalDelta = Total - PrevTotal;
    float IdleDelta = Idle - PrevIdle;

    float CPU_Percentage = (TotalDelta - IdleDelta) / TotalDelta;
    PrevIdle = Idle;
    PrevNonIdle = NonIdle;
    return CPU_Percentage;
}