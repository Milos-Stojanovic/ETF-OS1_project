/*
 * IVTEntry.cpp
 *
 *  Created on: 18.05.2020.
 *      Author: OS1
 */
#include "IVTEntry.h"
#include "system.h"
#include <dos.h>

IVTEntry* IVTEntry::newKeyboard;

IVTEntry::IVTEntry(int ivtNo, InterRout newRout){

/*#ifndef BCC_BLOCK_IGNORE*/
	this->ivtNo=ivtNo;
	newKeyboard=this;
	OldRout=0;
	myEvent=0;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
#ifndef BCC_BLOCK_IGNORE
	this->OldRout=getvect(ivtNo);
	setvect(ivtNo,newRout);
#endif

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void IVTEntry::setMyEvent(KernelEv* e){
	this->myEvent=e;
}

KernelEv* IVTEntry::retMyEvent(){
	return this->myEvent;
}


IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, OldRout);
#endif

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void IVTEntry::callOldInt() {
	OldRout();
}

void IVTEntry::signal() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	this->myEvent->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
