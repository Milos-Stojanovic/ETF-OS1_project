/*
 * semaphor.cpp
 *
 *  Created on: 07.05.2020.
 *      Author: OS1
 */

#include "semaphor.h"
#include"system.h"
#include<stdlib.h>
#include "KernelSe.h"
#include"SCHEDULE.H"


Semaphore::Semaphore(int init){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myImpl=new KernelSem(init);


#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

Semaphore::~Semaphore(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

		delete myImpl;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);

  
}

int Semaphore::signal(int n){ 
	return myImpl->signal(n);
	
}




int Semaphore::val()const{
	return myImpl->value();
}
