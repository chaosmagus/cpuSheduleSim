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
    Burst(int cpu_t, int io_t);
    int getCPU();
    int getIO();

private:
    int cpu, io;
};
#endif /* Burst_hpp */
