//
//

#include "Proc.hpp"
#include <iostream>

Proc::Proc(){}


Proc::Proc(ifstream &fin, int proc_id, int proc_type, int tCount){
    id = proc_id;
    type = proc_type;
    threadCount = tCount;
    cpu_time = 0;
    io_time = 0;

    //loop here to build thread queue for this proc using nxtThd()
    for(int i = 0; i < threadCount; i++){
       threads.push_back(this->nxtThd(fin, i));
    }
}

Thread* Proc::nxtThd(ifstream &fin, int thread_id){
    int t_arrive, bstCount; 
    fin >> t_arrive >> bstCount;
    Thread* thread = new Thread(fin, type, t_arrive, bstCount, thread_id, this->id);
    this->cpu_time += thread->getCPU();
    this->io_time += thread->getIO();
    //cout << "cpu: " << thread->getCPU() << " io: " << thread->getIO() << endl;
    return thread;
};

vector<Thread*>& Proc::getThreadQ(){
    return threads;
};

int Proc::getProcID(){
    return id;
};

string Proc::getProcType(){
    string tmp;
    switch(this->type){
        case 0: tmp = " [SYSTEM]:\n"; 
                break;
        case 1: tmp = " [INTERACTIVE]:\n";
                break;
        case 2: tmp = " [NORMAL]:\n";
                break;
        case 3: tmp = " [BATCH]:\n";
                break;
        default:
            break;
    }
    return tmp;
};

int Proc::getThdCnt(){
    return threadCount;
};

int Proc::proc_cpu(){
    return cpu_time;
};

int Proc::proc_io(){
    return io_time;
};

