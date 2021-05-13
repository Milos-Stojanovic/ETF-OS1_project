/*
 * IVTEntry.h
 *
 *  Created on: 18.05.2020.
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "KernelEv.h"

typedef void interrupt(*InterRout)(...);

class IVTEntry{

public:

	IVTEntry(int, InterRout);
	~IVTEntry();
	void callOldInt();
	void signal();
	KernelEv* retMyEvent();
	void setMyEvent(KernelEv* e);


private:
	friend class KernelEv;
	unsigned ivtNo;
	InterRout OldRout;
	static IVTEntry* newKeyboard;

	KernelEv* myEvent;
};



#endif /* IVTENTRY_H_ */
