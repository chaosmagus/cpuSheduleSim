//
//
//

#ifndef Burst_hpp
#define Burst_hpp

#include <stdio.h>
#include <math.h>

class Burst {
public:
    Burst();
    Burst(int cpu_t, int io_t, int last);
    int getCPU();
    void decrementCPU(int timeSlice);
    int getIO();
    bool isLast();

private:
    int cpu, io, lastBurst;
};
#endif /* Burst_hpp */
