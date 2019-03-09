//
//
//

#include <iostream>
#include "Simulation.hpp"
#include "Thread.hpp"
#include "Event.hpp"
#include "Proc.hpp"
#include "Burst.hpp"
#include <vector>
#include <fstream>
#include <stack>
#include <unistd.h>
//#include <getopt.h>
using namespace std;

int main(int argc, char * argv[]) {
    
    //initialize the file stream, process count, and switch costs
    ifstream fin;
    int proc_count, thread_cost, proc_cost;
    
    //process command line arguments
    if( argc == 2 ){ fin.open(argv[1]); }
    else {
        int opt = 0;
        while((opt = getopt(argc,argv,"ahtv")) != -1){
            switch (opt) {
            case 'a':
                cout << "flag was a\n";
                break;
            case 'h':
                cout << "flag was h\n";
                break;
            case 't':
                cout << "flag was t\n";
                break;
            case 'v':
                cout << "flag was v\n";
                break;
            default:
                break;
            }
        }
        fin.open(argv[argc-1]);
    }
    if(!fin.is_open()) cerr << "Error opening input file" << endl;
    fin >> proc_count >> thread_cost >> proc_cost;

    //create a simulation objecti, print the event queue
    Simulation mysim(fin, proc_count, thread_cost, proc_cost);
    mysim.printEvents();
     
    //loop through mysim's proc vec and cout to verify file parsing
    //vector<Proc*> tmp = mysim.getProcVec(); 
    /*for(int i = 0; i < tmp.size(); i++){
        cout << tmp[i]->getProcID() << " " << tmp[i]->getProcType() << " " <<tmp[i]->getThdCnt() << endl;
        queue<Thread*> threadQ = tmp[i]->getThreadQ();
        // thread arrival_t and burst_count
        while(!threadQ.empty()){
            cout << threadQ.front()->getArrival() << " " << threadQ.front()->getBurstCount() << endl;
            queue<Burst*> burstQ = threadQ.front()->getBurstQ();
            //burst lengths
            while(!burstQ.empty()){
                cout << burstQ.front()->getCPU() << " " << burstQ.front()->getIO() << endl;
                burstQ.pop();
            }
            threadQ.pop(); 
        }         
    }*/

    return 0;
}


