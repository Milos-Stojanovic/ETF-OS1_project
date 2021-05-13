/*
 * IDLE.h
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */
#ifndef IDLE_H_
#define IDLE_H_
#include "thread.h"

class IdleThread : public Thread {

public:
	void start(); 


	IdleThread();
	void run();

};


#endif /* IDLE_H_ */

