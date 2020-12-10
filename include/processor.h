#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

 private:
    float idle_{};
    float nonIdle_;
};

#endif