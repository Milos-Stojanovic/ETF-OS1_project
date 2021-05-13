/*
 * THREAD.cpp
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */


#include"thread.h"
#include"PCB.h"
#include "system.h"
#include<dos.h>
#include<stdlib.h>
#include "queue.h"
#include "SCHEDULE.H"
#include "KernelSe.h"
#include "stdio.h"

class system;
class Scheduler;

Thread::Thread(StackSize stackSize,Time timeSlice){


#ifndef BCC_BLOCK_IGNORE
	lock();
#endif


	myPCB=new PCB(this, stackSize, timeSlice);


#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


ID Thread::getId(){
	return myPCB->id;
}

ID Thread::getRunningId(){
	return system::running->id;
}

Thread* Thread::getThreadById(ID Id){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	queueElement* pom=system::q->firstEl;
	while(pom!=NULL){
		if((pom->PCBpointer->id==Id)){
			#ifndef BCC_BLOCK_IGNORE
				unlock();
			#endif
			return pom->PCBpointer->myThread;
		}
		pom=pom->next;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return NULL;
}

Thread::~Thread(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	waitToComplete();
	delete myPCB;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


void Thread::waitToComplete(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	if((myPCB->ThreadStatus==MADE)||(myPCB->ThreadStatus==FINISHED)){
		#ifndef BCC_BLOCK_IGNORE
			unlock();
		#endif
		return;
	}
	if(myPCB==(PCB*)system::running){
		#ifndef BCC_BLOCK_IGNORE
			unlock();
		#endif
			return;
	}
	if(this==system::starter){
		#ifndef BCC_BLOCK_IGNORE
			unlock();
		#endif
		return;
	}
	if(this==system::idle){
		#ifndef BCC_BLOCK_IGNORE
			unlock();
		#endif
		return;
	}

	system::running->ThreadStatus=BLOCKED;
	myPCB->waitQueue->put((PCB*)system::running);

#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
	system::dispatch();

}



void Thread::start(){

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	myPCB->ThreadStatus=READY;
	myPCB->makeStack(); //stek se kreira pri pokretanju niti

	system::q->put(myPCB);
	Scheduler::put(myPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	system::dispatch();

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}




