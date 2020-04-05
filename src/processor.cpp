#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <iostream>

using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    auto ut = LinuxParser::CpuUtilization();

    int user = ut[0];
    int nice = ut[1];
    int system = ut[2];
    int idle = ut[3];
    int iowait = ut[4];
    int irq = ut[5];
    int softirq = ut[6];
    int steal = ut[7];
    // int guest = ut[8];
    // int guest_nice = ut[9];

    int Idle = idle + iowait;
    int NonIdle = user + nice + system + irq + softirq + steal;
    int Total = Idle + NonIdle;
    float percent = (float)NonIdle / Total;
    return percent;
}