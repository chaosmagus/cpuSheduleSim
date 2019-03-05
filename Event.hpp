//
//

#ifndef Event_hpp
#define Event_hpp


#include "Thread.hpp"

enum EVENT_TYPE {THREAD_ARRIVED, DISPATCHER_INVOKED, THREAD_DISPATCH_COMPLETED, PROCESS_DISPATCH_COMPLETED, 
                CPU_BURST_COMPLETED, IO_BURST_COMPLETED, THREAD_COMPLETED, THREAD_PREEMPTED};


class Event {
public:
    Event(); //default constructor
    Event(int e_type, Thread* p_thread, int e_time);

    /*bool operator <( const Event &p) const{
        if(p.getX() == xval){
            if (p.getY() > yval) {return true;}
            else if(p.getY() <= yval) {return false;}
        }
        else if(p.getX() > xval) {return true;}
        else if(p.getX() <= xval) {return false;}
        
        return true;
    };*/
    
    // Get and set
    int getEventType();
    int getEventTime() const;
    Thread* getParentThread();
                    
private:
    int type;
    Thread* parentThread;
    int arrivalTime;

};
#endif /* Event_hpp */
