//
//
//

#include "Simulation.hpp"

Simulation::Simulation(){}

Simulation::Simulation(ifstream &fin, int pTotal, int tCost, int pCost){
    //inputFile = fin;
    procTotal = pTotal;
    txCost = tCost;
    pxCost = pCost;
    //loop here to build proc vector using nxtProc()
    for(int i = 0; i < procTotal; i++){
       procsToRun.push_back(this->nxtProc(fin)); 
    }
}

Proc* Simulation::nxtProc(ifstream &fin){
    int id, type, threadCount;
    fin  >> id >> type >> threadCount;
    Proc* proc = new Proc(fin, id, type, threadCount);
    switch (type) {
        case 0: this->sysTotal += threadCount;
                cout << sysTotal << " [SYSTEM]" << endl;
                break;
        case 1: this->intTotal += threadCount;
                cout << intTotal << " [INTERACTIVE]" << endl;
                break;
        case 2: this->normTotal += threadCount; 
                cout << normTotal << " [NORMAL]" << endl;
                break;
        case 3: this->batchTotal += threadCount;
                cout << batchTotal << " [BATCH]" << endl;
                break;
        default:
                cerr << "INVALID TYPE CODE" << endl;
                break;
        }
    //loop through proc threads, create events, add to pque
    for(int i = 0; i < proc->getThdCnt(); i++){
        int t = proc->getThreadQ()[i]->getBurstCount();
        Event* event = new Event(0, proc->getThreadQ()[i], proc->getThreadQ()[i]->getArrival());
        this->eventQ.push(event); 
    } 
    return proc;
};    

vector<Proc*> Simulation::getProcVec(){
    return procsToRun;
};

priority_queue<Event*, vector<Event*>, Compare> Simulation::getEventQ(){
    return eventQ;
};

void  Simulation::printEvents(){
    while(!eventQ.empty()){
        cout << "At Time " << eventQ.top()->getEventTime() << ":" << endl;
        int event_type = eventQ.top()->getEventType();
        switch (event_type) {
            case 0: cout << "\tTHREAD_ARRIVED" << endl;
                    break;
            case 1: cout << "\tTHREAD_DISPATCH_COMPLETED" << endl;
                    break;
            case 2: cout << "\tPROCESS_DISPATCH_COMPLETED" << endl;
                    break;
            case 3: cout << "\tCPU_BURST_COMPLETED" << endl;
                    break;
            case 4: cout << "\tIO_BURST_COMPLETED" << endl;
                    break;
            case 5: cout << "\tTHREAD_COMPLETED" << endl;
                    break;
            case 6: cout << "\tTHREAD_PREEMPTED" << endl;
                    break;
            case 7: cout << "\tDISPATCHER_INVOKED" << endl;
                    break;
            default:
                    cerr << "\tINVALID EVENT TYPE" << endl;
                    break;
        }
        cout << "\tThread " << eventQ.top()->getParentThread()->getThreadID() << " in process " << 
           eventQ.top()->getParentThread()->getParentID() << " ";
        int priority = eventQ.top()->getParentThread()->getPriority(); 
        switch (priority) {
            case 0: cout << "[SYSTEM]" << endl;
                    break;
            case 1: cout << "[INTERACTIVE]" << endl;
                    break;
            case 2: cout << "[NORMAL]" << endl;
                    break;
            case 3: cout << "[BATCH]" << endl;
                    break;
            default:
                    cerr << "INVALID PRIORITY CODE" << endl;
                    break;
        }
        //update event state
        cout << "\tTransitioned from NEW to READY" << endl << endl;
        eventQ.pop();
    }
};




