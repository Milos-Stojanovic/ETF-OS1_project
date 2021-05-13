/*
 * KernelSem.h
 *
 *  Created on: 07.05.2020.
 *      Author: OS1
 */

#ifndef KERNELSE_H_
#define KERNELSE_H_

#include"queue.h"





class KernelSem {
public:
	KernelSem(int m);
	virtual ~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n=0);
    void block();
    void deblock();
    int flag;
    int value()const;


    friend class system;
	int val;
	Queue* waitingQueue;
	Queue* blockedQueue;


};

#endif /* KERNELSE_H_ */
