//
//

#include "Event.hpp"

Event::Event(){}

Event::Event(int e_type, Thread* p_thread, int e_time){
    type = e_type;
    parentThread = p_thread;
    arrivalTime = e_time;
}

int Event::getEventType() { return type; };
int Event::getEventTime() const{ return arrivalTime; };
Thread* Event::getParentThread(){ return parentThread; };
