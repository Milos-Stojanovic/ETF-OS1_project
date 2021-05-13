/*
 * event.cpp
 *
 *  Created on: 18.05.2020.
 *      Author: OS1
 */

#include "event.h"
#include "KernelEv.h"
#include "system.h"
#include "IVTEntry.h"

Event::Event(IVTNo ivtNo){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myImpl=new KernelEv(ivtNo);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

Event::~Event(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete this->myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


void Event::wait(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myImpl->wait();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void Event::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	this->myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


