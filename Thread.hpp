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
    queue<Burst*>& getBurstQ();
    int getArrival();
    int getBurstCount();
    int getThreadID();
    int getPriority();
    int getParentID();
    int getResponseTime();
    int getTurnAroundTime();
    int getIO();    
    int getCPU();
    int getState();
    int getBlockTime() const;
    int getEndTime();
    int getStartTime(); 

    bool checkNew(); 
    void notNew(); 
    void updateState(int state);
    void setStartTime(int t);
    void setEndTime(int t);
    void setBlockTime(int t);
private:
    int arrivalTime, startTime, endTime, turnAroundTime, responseTime, cpuTime, ioTime, burstCount, thd_state,
         priority, id, parent_proc, blockTime; 
    //ifstream inputFile;
    queue<Burst*> bursts;
    bool isNew;
};

#endif /* Thread_hpp */
