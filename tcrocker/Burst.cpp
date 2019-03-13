
//
//
//

#include "Burst.hpp"

Burst::Burst(){}

Burst::Burst(int cpu_t, int io_t, int last){
    cpu = cpu_t;
    io = io_t;
    lastBurst = last;
}

int Burst::getCPU(){
    return cpu;
};

int Burst::getIO(){
    return io;
};

bool Burst::isLast(){
    if(this->lastBurst == 1) return true;
    return false;
};

