/*
 * PCB.h
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "queue.h"
#include "thread.h"
#include "dos.h"


	enum threadStatus{MADE, READY, BLOCKED, FINISHED};



class Queue;

class PCB {

public:
	friend class IdleThread;
	volatile int myTimerCounter;
	Time timeSlice;
	StackSize stackSize;
	Thread* myThread;


	volatile threadStatus ThreadStatus;
	ID id;
	static int id1;
	Queue* waitQueue;

	unsigned * stack;

	volatile Time myTime;
	int myReturnValue;

	volatile unsigned int sp;
	volatile unsigned int ss;
	void makeStack();


	PCB(Thread* myT , StackSize stackSize, Time timeSlice);
	~PCB();
};

#endif /* PCB_H_ */

