#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <unistd.h>

class Processor {
 public:
  float Utilization();

 private:
float PrevTotal{0};
float PrevIdle{0};
float User_Hz {(float) 1.0/sysconf(_SC_CLK_TCK)};
};

#endif