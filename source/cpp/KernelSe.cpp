/*
 * KernelSem.cpp
 *
 *  Created on: 07.05.2020.
 *      Author: OS1
 */

#include "KernelSe.h"
#include"SCHEDULE.H"
#include "system.h"
#include "queue.h"
#include "PCB.h"
#include "semaphor.h"
#include "stdio.h"


KernelSem::KernelSem(int m) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	this->val=m;
	this->waitingQueue=new Queue();
	this->blockedQueue=new Queue();
	this->flag=0;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}

void KernelSem::block(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	system::running->ThreadStatus=BLOCKED;
	if(system::running->myTime==0){
		blockedQueue->put(system::running);
	}
	else{
		system::running->myReturnValue=0;
		waitingQueue->put(system::running);
	}

	system::dispatch();
	

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void KernelSem::deblock(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	if(blockedQueue->br!=0){
		PCB* help=blockedQueue->get();
		help->ThreadStatus=READY;
		Scheduler::put(help);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return;
	}
		if(waitingQueue->br!=0){
				PCB* help=waitingQueue->get();
				help->myReturnValue=1;
				help->ThreadStatus=READY;
				Scheduler::put(help);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return;

	}
			


#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

KernelSem::~KernelSem() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	PCB* pomm; 
			while((pomm=blockedQueue->get())!=NULL){
				pomm->ThreadStatus=READY;
				Scheduler::put(pomm);
			}
			while((pomm=waitingQueue->get())!=NULL){
					pomm->ThreadStatus=READY;
					Scheduler::put(pomm);
				}
			

	delete blockedQueue;
	delete waitingQueue;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}








int KernelSem::wait(Time maxTimeToWait){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	system::running->myReturnValue=1;
	if(this->flag==0){
		this->flag=1;
		system::Lista->dodaj(this);
		}


	this->val--;
	if(this->val<0){
		system::running->myTime=maxTimeToWait;
			block();


	}

		if((waitingQueue->br==0)&&(blockedQueue->br==0)&&(this->flag==1)){
															this->flag=0;
														    system::Lista->izbaci(this);
														}

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

		return system::running->myReturnValue;

}


int KernelSem::signal(int n){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	if((this->flag==0)&&(n>=0)){
		this->flag=1;
			system::Lista->dodaj(this);
		}



	int pom=0;
	int freed=0;
	if(n<0){
		
	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
			return n;
		}


	if(n==0) {
		pom=1;
		n=1;
	}

	if(val>=0){
				val+=n;
				if((waitingQueue->br==0)&&(blockedQueue->br==0)){
						this->flag=0;
					    system::Lista->izbaci(this);
					}
		#ifndef BCC_BLOCK_IGNORE
			unlock();
		#endif
			return 0;
			}

	while((n>0)&&(val<0)){
		deblock();
		freed++;
		n--;
		this->val++;

	}
	if((waitingQueue->br==0)&&(blockedQueue->br==0)){
							this->flag=0;
						    system::Lista->izbaci(this);
						}

	if(n>0)
		this->val+=n;

	if(pom==1){
	
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return 0;
	}
	else{
		
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return freed;
	}
}


int KernelSem::value()const{
	return this->val;
}
