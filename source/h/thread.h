/*
 * THREAD.h
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */


#ifndef THREAD_H_
#define THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize=4096; //default velicina kontrolnog steka niti
typedef unsigned int Time; //time, x 55ms
const Time defaultTimeSlice=2; //dTS=2x55ms
typedef int ID;

class PCB;

class Thread{

public:
	void start(); 
	void waitToComplete();
	virtual ~Thread();

	ID getId(); 
	static ID getRunningId(); 
	static Thread* getThreadById(ID);

protected:
	friend class system;
	friend class PCB;
	friend class IdleThread;
	Thread(StackSize stackSize=defaultStackSize, Time timeSlice=defaultTimeSlice);
	virtual void run(){}

private:
	PCB* myPCB;
};

void dispatch(); 
void tick();

#endif /* THREAD_H_ */
