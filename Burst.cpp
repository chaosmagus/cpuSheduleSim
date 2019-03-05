
//
//
//

#include "Burst.hpp"

Burst::Burst(){}

Burst::Burst(int cpu_t, int io_t){
    cpu = cpu_t;
    io = io_t;
}

int Burst::getCPU(){
    return cpu;
};

int Burst::getIO(){
    return io;
};

