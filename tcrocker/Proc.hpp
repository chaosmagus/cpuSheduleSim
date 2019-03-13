//
//

#ifndef Proc_hpp
#define Proc_hpp

#include <vector>
#include "Thread.hpp"
using namespace std;
class Proc {
public:
    Proc();
    Proc(ifstream &fin, int proc_id, int proc_type, int tCount);
    Thread* nxtThd(ifstream &fin, int thread_id);
    vector<Thread*> getThreadQ();
    int getProcID();
    int getProcType();
    int getThdCnt();
    int proc_cpu();
    int proc_io();    

private:
    //ifstream inputFile;
    int id, type, threadCount, cpu_time, io_time;
    vector<Thread*> threads;
};
#endif /* Process_hpp */
