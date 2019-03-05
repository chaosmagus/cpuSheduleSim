//
//
//

#ifndef Simulation_hpp
#define Simulation_hpp

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

class Compare{
    public:
        int operator() (const Event* e1, const Event* e2){
            return e1->getEventTime() > e2->getEventTime();
        }
};

class Simulation {
public:
    Simulation();
    Simulation(ifstream &fin, int pTotal, int tCost, int pCost);
    
    //member functions: read data from file and pass to constructors
    Proc* nxtProc(ifstream &fin);    
    //Event* nxtEvent();
    //Burst* nxtBst();    
    vector<Proc*> getProcVec();
    void printEvents();

    priority_queue<Event*, vector<Event*>, Compare> getEventQ();
private:
    int procTotal, txCost, pxCost;
    vector<Proc*> procsToRun;
    priority_queue<Event*, vector<Event*>, Compare> eventQ;
};
#endif /* Simulation_hpp */
