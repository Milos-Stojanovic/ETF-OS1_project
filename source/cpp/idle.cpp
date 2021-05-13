/*
 * IDLE.cpp
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */



#include"idle.h"
#include"PCB.h"
#include "system.h"
#include<dos.h>
#include<stdlib.h>

#include <iostream.h>
#include <stdio.h>

#include "SCHEDULE.H"

class system;
class Scheduler;




IdleThread::IdleThread(){
	 Thread(256,1);
}







 void IdleThread::start(){

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	myPCB->ThreadStatus=READY;
	myPCB->makeStack(); //stek se kreira pri pokretanju idle niti
						// napravio sam da getThreadById ne moze da dohvati Idle nit 
	system::q->put(myPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void IdleThread::run(){
	while(1);
}





/*

IdleThread::~IdleThread(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	waitToComplete();
	delete myPCB;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
*/






