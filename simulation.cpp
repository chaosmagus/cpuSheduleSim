//
//
//

#include <iostream>
#include "Simulator.hpp"
#include "Thread.hpp"
#include "Event.hpp"
#include "Proc.hpp"
#include "Burst.hpp"
#include <vector>
#include <fstream>
#include <stack>
#include <unistd.h>
#include <algorithm>

using namespace std;

int main(int argc, char * argv[]) {

//initialize the file stream, process count, and switch costs
    ifstream fin;
    int proc_count, thread_cost, proc_cost;
    string algo = "fcfs"; 
    

//process command line arguments, check for error opening filestream
    if( argc == 2 ){ fin.open(argv[1]); }
    else {
        int opt = 0;
        string ln;
        while((opt = getopt(argc,argv,"a:htv")) != -1){
            switch (opt) {
            case 'a':
                algo = optarg;
                break;
            case 'h':
                fin.open("README.txt");;
                while(!fin.eof()){
                    getline(fin, ln); 
                    cout << ln << endl;
                }
                exit(1);
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


//get the total proc count, plus thread and proc switch cost, then initialize Simulator object
    fin >> proc_count >> thread_cost >> proc_cost;
    Simulator mysim(fin, proc_count, thread_cost, proc_cost);
 
//convert to lowercase, check valid entry, and run simulation with selected algorithm
    transform(algo.begin(), algo.end(), algo.begin(), ::tolower);
    if(algo == "fcfs"){
        mysim.runFCFS();
    } else if(algo == "rr"){
        //mysim.runRR(); 
    } else if(algo == "priority"){
        //mysim.runPRIORITY();
    } else if(algo == "custom"){
        //mysim.runCUSTOM();
    } else {
        cerr << "INVALID ALGORITHM ENTERED\n";
        exit(1);
    }
   



//THE FOLLOWING BLOCK USED FOR DEBUG DURING DEVELOPMENT
    //mysim.printEvents();
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


