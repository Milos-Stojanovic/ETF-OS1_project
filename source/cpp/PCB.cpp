/*
 * PCB.cpp
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */

#include "PCB.h"
#include "system.h"
#include <stdlib.h>
#include <dos.h>
#include"SCHEDULE.H"
#include "queue.h"
#include "stdio.h"

#include <iostream.h>

int PCB::id1=-1;


PCB::PCB(Thread* myT, StackSize stackSize, Time timeSlice){  //konstruktor PCB-a

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	this->timeSlice=timeSlice;
	this->stackSize=stackSize; //jer je word=1B, a  StackSize tip je velicine 2B => velicina polja u kontrolnom steku je ustvari 2B
	myThread=myT;
	waitQueue=new Queue();
	myTimerCounter=0;
	ThreadStatus=MADE;
	id=++id1;
	stack=NULL;
	myTime=-1; //?
	myReturnValue=1;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}


PCB::~PCB(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete waitQueue;
		delete stack;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}






void PCB::makeStack(){
#ifndef BCC_BLOCK_IGNORE
	lock();
		static int n = this->stackSize/2; //delim sa 2 jer je sizeof(unsigned)=2B
		stack = new unsigned[n];


		stack[n-1] = 0x200;//setovan I fleg u pocetnom PSW-u za nit

		stack[n-2] = FP_SEG(&(system::wrapper));
		stack[n-3] = FP_OFF(&(system::wrapper));

		this->sp = FP_OFF(stack+n-12); //svi sacuvani registri pri ulasku u interrupt rutinu (Milana rece na vezbama da se oni cuvaju implicitno)
		this->ss = FP_SEG(stack+n-12);


		unlock();
#endif
}




