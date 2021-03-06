//
//
//

#include "Simulator.hpp"

Simulator::Simulator(){}

Simulator::Simulator(ifstream &fin, int pTotal, int tCost, int pCost, bool v){
    procTotal = pTotal;
    txCost = tCost;
    pxCost = pCost;
    totalTime = 0;
    currentProcID = -1;
    timeSlice = 3;
    verbose = v;
    sysRT = sysTAT = sysTotal = intTotal = intRT = intTAT = normRT = normTAT = normTotal = batchTotal = batchRT = batchTAT = totalTime = ioTime = cpuTime = dispTime = idle = 0; 
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

void Simulator::incrementTime(int t, int algorithm){
    totalTime += t;
    thread_arrived(algorithm);
    if(!blocked.empty()){io_burst(algorithm);}
};

void Simulator::thread_arrived(int algorithm){
    //check the thread_arrived pque for new threads
    Event* e = arrivalEventQ.top();
    switch (algorithm) {
        case 0: fcfs_arrival(e);
                break;
        case 1: priority_arrival(e);
                break;
        case 2: //rr_arrival(e);
                break;
        case 3: //custom_arrival;
                break;
        default:
                cerr << "SOMETHING HAS GONE HORRIBLY WRONG\n" << endl;
                break;
    }
};
void Simulator::fcfs_arrival(Event* e){
    while(e->getEventTime() <= totalTime && !(arrivalEventQ.empty())){
        if(verbose){printEvent(e->getEventTime(), e, 0);}
        events.push(e);
        ready.push(e->getParentThread());
        arrivalEventQ.pop();
        e = arrivalEventQ.top();
    }
};

void Simulator::priority_arrival(Event* e){
    while(e->getEventTime() <= totalTime && !(arrivalEventQ.empty())){
        if(verbose){printEvent(e->getEventTime(), e, 1);}
        events.push(e);
        int priority = e->getParentThread()->getPriority();
        switch(priority){
            case 0:
                sysReady.push(e->getParentThread());
                break;
            case 1:
                intReady.push(e->getParentThread());
                break;
            case 2:
                normReady.push(e->getParentThread());
                break;
            case 3:
                batchReady.push(e->getParentThread());
                break;
            default:
                cerr << "SOMETHING HAS GONE HORRIBLY WRONG\n";
                break;
        }
        arrivalEventQ.pop();
        e = arrivalEventQ.top();
    }

};

void Simulator::dispatch_invoked(Thread* t, int algorithm){
    //create event and push to event queue
    Event* e = new Event (7, t, totalTime);
    events.push(e);
    if(verbose){printEvent(totalTime, e, algorithm);} 
    //increment time step
    if(t->getParentID() != currentProcID){ 
        incrementTime(pxCost, algorithm);
        dispTime += pxCost;    
    } else {
        incrementTime(txCost, algorithm);
        dispTime += txCost;
    }
};

void Simulator::thd_dispatch_complete(Thread* t, int algorithm){
    //set thd starting time, update state to ready/run(ie '1')
    Event* e = new Event(1, t, totalTime);    
    currentProcID = t->getParentID();
    if(t->checkNew()){
        t->setStartTime(totalTime);  
        t->notNew();
    }
    t->updateState(1);
    events.push(e);
    if(verbose){printEvent(totalTime, e, algorithm);}

};

void Simulator::proc_dispatch_complete(Thread* t, int algorithm){
    //set thd starting time, update state to ready/run(ie '1')
    Event* e = new Event(2, t, totalTime);    
    currentProcID = t->getParentID();
    if(t->checkNew()){
        t->setStartTime(totalTime);  
        t->notNew();
    }
    t->updateState(1);
    events.push(e);
    if(verbose){printEvent(totalTime, e, algorithm);}
};

void Simulator::cpu_burst(Thread* t, int algorithm){
     if((algorithm == 2) && (t->getBurstQ().front()->getCPU() > timeSlice)){
            t->getBurstQ().front()->decrementCPU(timeSlice); 
            incrementTime(timeSlice, algorithm);
            t->updateState(5); 
            Event* e = new Event(6, t, totalTime);    
            events.push(e);
            if(verbose){printEvent(totalTime, e, algorithm);}
            ready.push(t);
            //ready.pop();
        
    } else {    
        incrementTime(t->getBurstQ().front()->getCPU(), algorithm);    
        //if this burst is the last burst in the thread, set end time, update state to run/exit(ie '5')
        if(t->getBurstQ().front()->isLast()){
            t->setEndTime(totalTime);
            t->updateState(4);       
            int p = t->getPriority();
            switch (p) {
                case 0: this->sysTAT += (totalTime - t->getArrival());
                        //cout << sysTotal << " [SYSTEM]" << endl;
                        break;
                case 1: this->intTAT += (totalTime - t->getArrival());
                        //cout << intTotal << " [INTERACTIVE]" << endl;
                        break;
                case 2: this->normTAT += (totalTime - t->getArrival());  
                        //cout << normTotal << " [NORMAL]" << endl;
                        break;
                case 3: this->batchTAT += (totalTime - t->getArrival()); 
                        //cout << batchTotal << " [BATCH]" << endl;
                        break;
                default:
                        cerr << "INVALID TYPE CODE DIS VERY BAD!" << endl;
                        break;
            }
            Event* e = new Event(5, t, totalTime);    
            events.push(e);
            if(verbose){printEvent(totalTime, e, algorithm);}
        } else {
        //otherwise update state to run/blk (ie '2'), push thread onto 'blocked' queue
            t->updateState(2);
            Event* e = new Event(3, t, totalTime);    
            t->setBlockTime(totalTime + t->getBurstQ().front()->getIO());
            blocked.push(t);
            events.push(e);
            //cout << "CPU TIME " << blocked.top()->getBurstQ().front()->getCPU() << endl;
            if(verbose){printEvent(totalTime, e, algorithm);}
        }
    }
};

void Simulator::io_burst(int algorithm){
    //check the blocked pque for threads ready to be moved to 'ready', pop burst from burst queue
    while(blocked.top()->getBlockTime() <= totalTime && !(blocked.empty())){
        blocked.top()->updateState(3);
        Event* e = new Event(4, blocked.top(), blocked.top()->getBlockTime());    
        events.push(e);
        if(verbose){ printEvent(blocked.top()->getBlockTime(), e, algorithm);}
       // cout << "IO TIME before" << blocked.top()->getBurstQ().front()->getIO() << endl;
        blocked.top()->getBurstQ().pop();
        //cout << "IO TIME after" << blocked.top()->getBurstQ().front()->getIO() << endl;
        int p = blocked.top()->getPriority();
        switch (algorithm) {
            case 0: ready.push(blocked.top());
                    break;
            case 1:
                    switch(p){
                        case 0:
                            sysReady.push(e->getParentThread());
                            break;
                        case 1:
                            intReady.push(e->getParentThread());
                            break;
                        case 2:
                            normReady.push(e->getParentThread());
                            break;
                        case 3:
                            batchReady.push(e->getParentThread());
                            break;
                        default:
                            cerr << "SOMETHING HAS GONE HORRIBLY WRONG\n";
                            break;
                    }
                    break;
            case 2: ready.push(blocked.top());
                    break;
            case 3: //custom_arrival;
                    break;
            default:
                    cerr << "SOMETHING HAS GONE HORRIBLY WRONG\n" << endl;
                    break;
        }
         blocked.pop();
    }
};

void Simulator::runFCFS(){
    //enque any new threads that have arrived, call dispatcher, loop through the ready queue
    thread_arrived(0);
    while(!ready.empty()){  
        dispatch_invoked(ready.front(), 0);
        if(ready.front()->getParentID() != currentProcID){ proc_dispatch_complete(ready.front(), 0);}
        else { thd_dispatch_complete(ready.front(), 0); }
        cpu_burst(ready.front(), 0);
        Thread* tmp = ready.front();
        ready.pop();
        //if readyQ is empty but blocked is not, increment time by the IO burst at front of blockedQ
        if(ready.empty() && !(blocked.empty())){incrementTime(tmp->getBurstQ().front()->getIO(), 0);}
        //if readyQ is empty but arrivalEventQ is not, increment time and get the new thread. 
        if(ready.empty() && !(arrivalEventQ.empty())){
            cout << "in the loop" << endl;
            incrementTime((arrivalEventQ.top()->getEventTime()) - totalTime, 0);}
    }
};

void Simulator::runRR(){
    //enque any new threads that have arrived, call dispatcher, loop through the ready queue
    thread_arrived(0);
    while(!ready.empty()){  
        dispatch_invoked(ready.front(), 0);
        if(ready.front()->getParentID() != currentProcID){ proc_dispatch_complete(ready.front(), 0);}
        else { thd_dispatch_complete(ready.front(), 0); }
        cpu_burst(ready.front(), 2);
        Thread* tmp = ready.front();
        ready.pop();
        //if readyQ is empty but blocked is not, increment time by the IO burst at front of blockedQ
        if(ready.empty() && !(blocked.empty())){incrementTime(tmp->getBurstQ().front()->getIO(), 0);}
        //if readyQ is empty but arrivalEventQ is not, increment time and get the new thread. 
        if(ready.empty() && !(arrivalEventQ.empty())){
            cout << "in the loop" << endl;
            incrementTime((arrivalEventQ.top()->getEventTime()) - totalTime, 0);}
    }

};


void Simulator::runPRIORITY(){
    //enque any new threads that have arrived, call dispatcher, loop through the ready queue
    thread_arrived(1);
    Thread* tmp;
    while(!(sysReady.empty()) || !(intReady.empty()) || !(normReady.empty()) || !(batchReady.empty())){
        if(!(sysReady.empty())){
            dispatch_invoked(sysReady.front(), 1);
            if(sysReady.front()->getParentID() != currentProcID){ proc_dispatch_complete(sysReady.front(), 1);}
            else { thd_dispatch_complete(sysReady.front(), 1); }
            cpu_burst(sysReady.front(), 1);
            tmp = sysReady.front();
            sysReady.pop();
            
        } else if (!(intReady.empty())) {
            dispatch_invoked(intReady.front(), 1);
            if(intReady.front()->getParentID() != currentProcID){ proc_dispatch_complete(intReady.front(), 1);}
            else { thd_dispatch_complete(intReady.front(), 1); }
            cpu_burst(intReady.front(), 1);
            tmp = intReady.front();
            intReady.pop();

        } else if(!(normReady.empty())){
            dispatch_invoked(normReady.front(), 1);
            if(normReady.front()->getParentID() != currentProcID){ proc_dispatch_complete(normReady.front(), 1);}
            else { thd_dispatch_complete(normReady.front(), 1); }
            cpu_burst(normReady.front(), 1);
            tmp = normReady.front();
            normReady.pop();

        } else if(!(batchReady.empty())){
            dispatch_invoked(batchReady.front(), 1);
            if(batchReady.front()->getParentID() != currentProcID){ proc_dispatch_complete(batchReady.front(), 1);}
            else { thd_dispatch_complete(batchReady.front(), 1); }
            cpu_burst(batchReady.front(), 1);
            tmp = batchReady.front();
            batchReady.pop();

        }


        //if readyQueues are empty but blocked is not, increment time by the IO burst at front of blockedQ
        if(sysReady.empty() && intReady.empty() && normReady.empty() && batchReady.empty() && !(blocked.empty())){
            incrementTime(tmp->getBurstQ().front()->getIO(), 1);
        }
        //if readyQueues are empty but arrivalEventQ is not, increment time and get the new thread. 
        if(sysReady.empty() && intReady.empty() && normReady.empty() && batchReady.empty() && !(arrivalEventQ.empty())){
            cout << "in the loop" << endl;
            incrementTime((arrivalEventQ.top()->getEventTime()) - totalTime, 1);}
    }






    /*while(!ready.empty()){  
        dispatch_invoked(ready.front(), 1);
        if(ready.front()->getParentID() != currentProcID){ proc_dispatch_complete(ready.front());}
        else { thd_dispatch_complete(ready.front()); }
        cpu_burst(ready.front(), 1);
        Thread* tmp = ready.front();
        ready.pop();
        //if readyQ is empty but blocked is not, increment time by the IO burst at front of blockedQ
        if(ready.empty() && !(blocked.empty())){incrementTime(tmp->getBurstQ().front()->getIO(), 1);}
        //if readyQ is empty but arrivalEventQ is not, increment time and get the new thread. 
        if(ready.empty() && !(arrivalEventQ.empty())){
            cout << "in the loop" << endl;
            incrementTime((arrivalEventQ.top()->getEventTime()) - totalTime, 1);}
    }*/

};

void Simulator::runCUSTOM(){

};
void  Simulator::printEvent(int time, Event* e, int algorithm){
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
        //print thread state change or the dispatch choice
        if(event_type == 7){
            switch (algorithm) {
                case 0: cout << "\tSelected from " << ready.size() << " threads\n\n";
                        break;
                case 1:
                        switch(priority){
                            case 0:
                                cout << "\tSelected from " << sysReady.size() << " threads\n\n";
                                break;
                            case 1:
                                cout << "\tSelected from " << intReady.size() << " threads\n\n";
                                break;
                            case 2:
                                cout << "\tSelected from " << normReady.size() << " threads\n\n";
                                break;
                            case 3:
                                cout << "\tSelected from " << batchReady.size() << " threads\n\n";
                                break;
                            default:
                                cerr << "SOMETHING HAS GONE HORRIBLY WRONG\n";
                                break;
                        }
                        break;
                case 2: //rr_arrival(e);
                        break;
                case 3: //custom_arrival;
                        break;
                default:
                        cerr << "SOMETHING HAS GONE HORRIBLY WRONG\n" << endl;
                        break;
            }

        } else {
            int state = e->getParentThread()->getState();
            cout << "\tTransitioned from ";
            switch (state) {
                case 0: cout << "NEW to READY\n" << endl;
                        break;
                case 1: cout << "READY to RUNNING\n" << endl;
                        break;
                case 2: cout << "RUNNING to BLOCKED\n" << endl;
                        break;
                case 3: cout << "BLOCKED to READY\n" << endl;
                        break;
                case 4: cout << "RUNNING to EXIT\n" << endl;
                        break;
                case 5: cout << "RUNNING to READY\n" << endl;
                        break;
                default:
                        cerr << "INVALID STATE\n" << endl;
                        break;
            }
        }
};

void Simulator::printStats(){
    calcResponse();
    cout << "SYSTEM THREADS:" << endl;
    cout << "\tTotal count: " << sysTotal << endl;
    cout << "\tAvg response time: " << sysRT << endl;
    cout << "\tAvg turnaround time: " << sysTAT/sysTotal << endl << endl;
    
    cout << "INTERACTIVE THREADS:" << endl;
    cout << "\tTotal count: " << intTotal << endl;
    cout << "\tAvg response time: " << intRT << endl;
    cout << "\tAvg turnaround time: " << intTAT/intTotal << endl << endl;

    cout << "NORMAL THREADS:" << endl;
    cout << "\tTotal count: " << normTotal << endl;
    cout << "\tAvg response time: " << normRT << endl;
    cout << "\tAvg turnaround time: " << normTAT/normTotal << endl << endl;

    cout << "BATCH THREADS:" << endl;
    cout << "\tTotal count: " << batchTotal << endl;
    cout << "\tAvg response time: " << batchRT << endl;
    cout << "\tAvg turnaround time: " << batchTAT/batchTotal << endl << endl;

    cout << "Total elapsed time: " << totalTime << endl;
    cout << "Total service time: " << cpuTime << endl;
    cout << "Total I/O time: " << ioTime << endl;
    cout << "Total dispatch time: " << dispTime << endl;
    cout << "Total idle time: " << (totalTime - cpuTime - dispTime) << endl << endl;

    double efficiency = 100 * cpuTime/totalTime;
    double utilization = 100 * (cpuTime + dispTime)/totalTime; 
    cout << "CPU utilization: "<< utilization << " %" << endl;
    cout << "CPU efficiency: "<< efficiency << " %" << endl << endl;
};


void Simulator::threadStats(){
    for(int i = 0; i < procsToRun.size(); i++){
        cout << "Process " << i << procsToRun[i]->getProcType(); 
        for(int j = 0; j < procsToRun[i]->getThdCnt(); j++){
            cout << "\tThread " << j << ":\tARR: " << procsToRun[i]->getThreadQ()[j]->getArrival() << "\t\tCPU: " << procsToRun[i]->getThreadQ()[j]->getCPU() << "\t\tI/O: " << procsToRun[i]->getThreadQ()[j]->getIO() << "\t\tTRT: " << procsToRun[i]->getThreadQ()[j]->getTurnAroundTime() << " \tEND: " << procsToRun[i]->getThreadQ()[j]->getEndTime() << endl; 
        }
    }
};

void Simulator::calcResponse(){
    double system(0), interactive(0), normal(0),  batch(0);
    for(int i = 0; i < procsToRun.size(); i++){
        for(int j = 0; j < procsToRun[i]->getThdCnt(); j++){
            int priority = procsToRun[i]->getThreadQ()[j]->getPriority(); 
            double response = (procsToRun[i]->getThreadQ()[j]->getStartTime() - procsToRun[i]->getThreadQ()[j]->getArrival()); 
            switch (priority) {
                case 0: system += response;
                        break;
                case 1: interactive += response;
                        break;
                case 2: normal += response;
                        break;
                case 3: batch += response;
                        break;
                default:
                        cerr << "INVALID PRIORITY CODE" << endl;
                        break;
            }
        }
    }
    cout << sysTotal << " " << system <<  " " << endl;
    this->sysRT = system/sysTotal;
    this->intRT = interactive/intTotal;
    this->normRT = normal/normTotal;
    this->batchRT = batch/batchTotal;
};




