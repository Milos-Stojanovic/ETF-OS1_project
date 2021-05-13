/*
 * event.h
 *
 *  Created on: 18.05.2020.
 *      Author: OS1
 */


#ifndef EVENT_H_
#define EVENT_H_

#include "IVTEntry.h"

#define PREPAREENTRY(ivtno,old)\
	void interrupt routine##ivtno(...);\
	IVTEntry ent##ivtno(ivtno,routine##ivtno);\
	void interrupt routine##ivtno(...) {\
		if(old) ent##ivtno.callOldInt();\
		ent##ivtno.signal();\
		dispatch();\
	}

typedef unsigned char IVTNo;
class KernelEv;


class Event{
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;
};

#endif /* EVENT_H_ */


