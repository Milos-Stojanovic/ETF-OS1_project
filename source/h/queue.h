/*
 * QUEUE.h
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */

#ifndef QUEUE_H_
#define QUEUE_H_


#include"PCB.h"
#include<stdlib.h>
#include"thread.h"

class PCB;


class queueElement{
public:
	queueElement* next;
	PCB* PCBpointer;
	queueElement(PCB* p);
};



class Queue {

public:
	void put(PCB*);
	PCB* get();



	int br;
	Queue(); 
	~Queue();
	queueElement* firstEl;
	queueElement* lastEl;//first - deo za dodavanje
						 //last - deo za uzimanje

};

#endif /* QUEUE_H_ */

