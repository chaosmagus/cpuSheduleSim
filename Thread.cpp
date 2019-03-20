//
//

#include "Thread.hpp"

Thread::Thread(){}


Thread::Thread(ifstream &fin, int thd_priority, int t_arrival, int bCount, int thread_id, int pproc){
    arrivalTime = t_arrival;
    burstCount = bCount;
    priority = thd_priority;
    thd_state = 0;
    id = thread_id;
    parent_proc = pproc;
    cpuTime = 0;
    ioTime = 0; 
    //loop to add bursts
    for(int i = 0; i < (bCount - 1); i ++){
        bursts.push(this->nxtBst(fin));
    }
    //add the last cpu burst
    int finalBurst;
    fin >> finalBurst;
    Burst* final = new Burst(finalBurst, 0, 1);
    this->cpuTime += finalBurst;
    bursts.push(final);
}

void Thread::setBlockTime(int t){
    blockTime = t;
};

int Thread::getBlockTime() const{
    return blockTime;
};

Burst* Thread::nxtBst(ifstream &fin){
    int t_cpu, t_io; 
    fin >> t_cpu >> t_io;
    this->cpuTime += t_cpu;
    this->ioTime += t_io;
    Burst* burst = new Burst(t_cpu, t_io, 0);
    return burst;
};

queue<Burst*>& Thread::getBurstQ(){
    return bursts; 
};

int Thread::getArrival(){
    return arrivalTime;
};

int Thread::getBurstCount(){
    return burstCount;
};

int Thread::getThreadID(){
    return id;
};

int Thread::getPriority(){
    return priority;
};

int Thread::getParentID(){
    return parent_proc;
};

int Thread::getResponseTime(){
    return responseTime;
};

int Thread::getTurnAroundTime(){
    return turnAroundTime; 
};

int Thread::getIO(){
    return ioTime;
};    

int Thread::getCPU(){
    return cpuTime;
};

int Thread::getState(){
    return thd_state;
};

void Thread::setStartTime(int t){
    startTime = t;
};

void Thread::setEndTime(int t){
    endTime = t;
};

void Thread::updateState(int state){
    thd_state = state;
};
