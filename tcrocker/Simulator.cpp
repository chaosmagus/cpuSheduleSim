//
//
//

#include "Simulator.hpp"

Simulator::Simulator(){}

Simulator::Simulator(ifstream &fin, int pTotal, int tCost, int pCost){
    procTotal = pTotal;
    txCost = tCost;
    pxCost = pCost;
    totalTime = 0;
    //loop here to build proc vector using nxtProc()
    for(int i = 0; i < procTotal; i++){
       procsToRun.push_back(this->nxtProc(fin)); 
    }
    //cout << cpuTime << "\t" << ioTime << endl;
}

//get next proc, increment io and cpu time, and the thread count for each proc type
Proc* Simulator::nxtProc(ifstream &fin){
    int id, type, threadCount;
    fin  >> id >> type >> threadCount;
    Proc* proc = new Proc(fin, id, type, threadCount);
    this->ioTime += proc->proc_io();
    this->cpuTime += proc->proc_cpu();
    switch (type) {
        case 0: this->sysTotal += threadCount;
                //cout << sysTotal << " [SYSTEM]" << endl;
                break;
        case 1: this->intTotal += threadCount;
                //cout << intTotal << " [INTERACTIVE]" << endl;
                break;
        case 2: this->normTotal += threadCount; 
                //cout << normTotal << " [NORMAL]" << endl;
                break;
        case 3: this->batchTotal += threadCount;
                //cout << batchTotal << " [BATCH]" << endl;
                break;
        default:
                cerr << "INVALID TYPE CODE" << endl;
                break;
        }
    //loop through proc threads, create events, add to pque
    for(int i = 0; i < proc->getThdCnt(); i++){
        int t = proc->getThreadQ()[i]->getBurstCount();
        Event* event = new Event(0, proc->getThreadQ()[i], proc->getThreadQ()[i]->getArrival());
        this->arrivalEventQ.push(event); 
    } 
    return proc;
};    

vector<Proc*> Simulator::getProcVec(){
    return procsToRun;
};

priority_queue<Event*, vector<Event*>, Compare> Simulator::getEvents(){
    return events;
};

priority_queue<Event*, vector<Event*>, Compare> Simulator::getArrivalEventQ(){
    return arrivalEventQ;
};

void Simulator::thread_arrived(){
    Event* e = arrivalEventQ.top();
    while(e->getEventTime() <= totalTime){
        printEvent(e->getEventTime(), e);
        events.push(e);
        arrivalEventQ.pop();
        e = arrivalEventQ.top();
    }
};

void Simulator::runFCFS(){
    //enque any new threads that have arrived, call dispatcher
    thread_arrived();
    //invoke_dispatch();
};

void Simulator::runRR(){

};


void Simulator::runPRIORITY(){

};

void Simulator::runCUSTOM(){

};
void  Simulator::printEvent(int time, Event* e){
        cout << "At Time " << time << ":" << endl;
        int event_type = e->getEventType();
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
        cout << "\tThread " << e->getParentThread()->getThreadID() << " in process " << 
           e->getParentThread()->getParentID() << " ";
        int priority = e->getParentThread()->getPriority(); 
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
        //print thread state change
        int state = e->getParentThread()->getState();
        cout << "\tTransitioned from ";
        switch (state) {
            case 0: cout << "NEW to READY" << endl;
                    break;
            case 1: cout << "READY to RUNNING" << endl;
                    break;
            case 2: cout << "RUNNING to BLOCKED" << endl;
                    break;
            case 3: cout << "BLOCKED to READY" << endl;
                    break;
            default:
                    cerr << "INVALID STATE" << endl;
                    break;
        }
};




