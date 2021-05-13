/*
 * semaphor.h
 *
 *  Created on: 07.05.2020.
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

typedef unsigned int Time;

class KernelSem;

class Semaphore{
public:
	Semaphore(int init=1);
	virtual ~Semaphore();

	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n=0);

	int val()const;

private:
	KernelSem* myImpl;
};



#endif /* SEMAPHOR_H_ */
