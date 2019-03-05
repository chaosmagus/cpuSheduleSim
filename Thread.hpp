//
//

#ifndef Thread_hpp
#define Thread_hpp

#include <queue>
#include <fstream>
#include <queue>
#include "Burst.hpp"

using namespace std;

class Thread {
public:
    Thread();
    Thread(ifstream &fin, int thd_priority, int t_arrival, int bCount, int thread_id, int pproc);
    Burst* nxtBst(ifstream &fin);
    queue<Burst*> getBurstQ();
    int getArrival();
    int getBurstCount();
    int getThreadID();
    int getPriority();
    int getParentID();

private:
    int arrivalTime, burstCount, thd_state, priority, id, parent_proc; 
    //ifstream inputFile;
    queue<Burst*> bursts;
};

#endif /* Thread_hpp */
