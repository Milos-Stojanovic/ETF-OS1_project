/*
 * KernelEv.h
 *
 *  Created on: 18.05.2020.
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

typedef unsigned char IVTNo;

class Thread;
class PCB;
class KernelSem;

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	int wait();
	void signal();

private:
	IVTNo ivtNo;
	KernelSem* mySem;
	PCB* owner;
};



#endif /* KERNELEV_H_ */
