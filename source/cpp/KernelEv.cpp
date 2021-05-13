/*
 * KernelEv.cpp
 *
 *  Created on: 18.05.2020.
 *      Author: OS1
 */

#include "KernelEv.h"
#include"IVTEntry.h"
#include"KernelSe.h"
#include"system.h"

KernelEv::KernelEv(IVTNo ivtNo){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(IVTEntry::newKeyboard!=NULL){
		this->ivtNo=ivtNo;
		this->mySem=new KernelSem(0);
		this->owner=system::running; //ILI THREAD-MOGUCA GRESKA
		IVTEntry::newKeyboard->setMyEvent(this);
	}

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

KernelEv::~KernelEv(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete mySem;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

int KernelEv::wait(){

	if(system::running!=owner) return 0;

	return this->mySem->wait(0);
}

void KernelEv::signal() {
	if (mySem->value()==1) return;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	mySem->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
