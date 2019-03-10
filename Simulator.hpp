//
//
//

#ifndef Simulator_hpp
#define Simulator_hpp

#include <stdio.h>
#include <math.h>
#include <vector>
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

class Simulator {
public:
    Simulator();
    Simulator(ifstream &fin, int pTotal, int tCost, int pCost);
    
    //member functions
    Proc* nxtProc(ifstream &fin);    
    vector<Proc*> getProcVec();
    void printEvents();
    void runFCFS();
    void runRR();
    void runPRIORITY();
    void runCUSTOM();
    void getNewArrivals();

    priority_queue<Event*, vector<Event*>, Compare> getEvents();
    priority_queue<Event*, vector<Event*>, Compare> getArrivalEventQ();
private:
    int procTotal, txCost, pxCost, sysRT, sysTAT, sysTotal, intTotal, intRT, intTAT, normRT, normTAT, 
        normTotal, batchTotal, batchRT, batchTAT, totalTime, ioTime, cpuTime, dispTime, idle;
    vector<Proc*> procsToRun;
    priority_queue<Event*, vector<Event*>, Compare> arrivalEventQ;
    priority_queue<Event*, vector<Event*>, Compare> events;
};
#endif /* Simulator_hpp */
