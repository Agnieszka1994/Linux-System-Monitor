#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
public:
    
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization() const;            
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(const Process& a) const;  
  bool operator>(const Process& a) const;
    void updateID(int);

  
private:
    int pid_{0};
};

#endif