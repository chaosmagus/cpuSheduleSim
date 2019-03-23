//
//
//

#ifndef Simulator_hpp
#define Simulator_hpp

#include <stdio.h>
#include <math.h>
#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include "Proc.hpp"
#include "Thread.hpp"
#include "Burst.hpp"
#include "Event.hpp"
#include <functional>
#include <queue>
#include <string>

class Compare{
    public:
        int operator() (const Event* e1, const Event* e2){
            return e1->getEventTime() > e2->getEventTime();
        }
};
class CompareT{
    public:
        int operator() (const Thread* t1, const Thread* t2){
            return t1->getBlockTime() > t2->getBlockTime();
        }
};

class Simulator {
public:
    Simulator();
    Simulator(ifstream &fin, int pTotal, int tCost, int pCost, bool v);
    
    //member functions
    Proc* nxtProc(ifstream &fin);    
    vector<Proc*> getProcVec();
    void printEvent(int time, Event* e, int algorithm);
    void runFCFS();
    void runRR();
    void runPRIORITY();
    void runCUSTOM();
    void thread_arrived(int algorithm);
    void dispatch_invoked(Thread* t, int algorithm);
    void proc_dispatch_complete(Thread* t, int algorithm);
    void thd_dispatch_complete(Thread* t, int algorithm);
    void io_burst(int algorithm);
    void cpu_burst(Thread* t, int algorithm);
    void cleanup();
    void incrementTime(int t, int algorithm);
    void fcfs_arrival(Event* e);
    void priority_arrival(Event* e);
    void printStats();
    void threadStats();
    void calcResponse();

    priority_queue<Event*, vector<Event*>, Compare> getEvents();
    priority_queue<Event*, vector<Event*>, Compare> getArrivalEventQ();
private:
    bool verbose;
    double timeSlice, procTotal, txCost, pxCost, sysRT, sysTAT, sysTotal, intTotal, intRT, intTAT, normRT, normTAT, normTotal, batchTotal, batchRT, batchTAT, totalTime, ioTime, cpuTime, dispTime, idle, currentProcID;
    vector<Proc*> procsToRun;
    priority_queue<Thread*, vector<Thread*>, CompareT> blocked;
    priority_queue<Event*, vector<Event*>, Compare> arrivalEventQ;
    queue<Thread*> ready, sysReady, intReady, normReady, batchReady;
    priority_queue<Event*, vector<Event*>, Compare> events;
};
#endif /* Simulator_hpp */
