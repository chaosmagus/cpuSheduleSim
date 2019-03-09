//
//

#include "Proc.hpp"
#include <iostream>

Proc::Proc(){}


Proc::Proc(ifstream &fin, int proc_id, int proc_type, int tCount){
    //inputFile = fin;
    id = proc_id;
    type = proc_type;
    threadCount = tCount;
    //loop here to build thread queue for this proc using nxtThd()
    for(int i = 0; i < threadCount; i++){
       threads.push_back(this->nxtThd(fin, i));
    }
}

Thread* Proc::nxtThd(ifstream &fin, int thread_id){
    int t_arrive, bstCount; 
    fin >> t_arrive >> bstCount;
    Thread* thread = new Thread(fin, type, t_arrive, bstCount, thread_id, this->id);
    //cout << "cpu: " << thread->getCPU() << " io: " << thread->getIO() << endl;
    return thread;
};

vector<Thread*> Proc::getThreadQ(){
    return threads;
};

int Proc::getProcID(){
    return id;
};

int Proc::getProcType(){
    return type;
};

int Proc::getThdCnt(){
    return threadCount;
};

