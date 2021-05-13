/*
 * QUEUE.cpp
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */


#include "queue.h"
#include"SCHEDULE.H"
#include"system.h"
#include <stdlib.h>


void Queue::put(PCB* ppp){ 

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	br++;
	queueElement* elemm=new queueElement(ppp); 

	if(firstEl==NULL){ //slucaj praznog reda
		lastEl=elemm;
	}
	else{ //slucaj red nije prazan
		elemm->next=firstEl;
	}
	firstEl=elemm;

	
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

Queue::Queue(){ 
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	firstEl=lastEl=NULL;
	br=0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


Queue::~Queue(){  

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(firstEl!=lastEl){
		while(firstEl!=NULL){
			queueElement* pom12=firstEl;
			firstEl=firstEl->next;
			pom12->next=NULL;
			delete pom12;
		}
		lastEl=NULL;
	}
	else{
		if(firstEl!=NULL){
			queueElement* pom=firstEl;
			firstEl=lastEl=NULL;
			delete pom;
		}
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


PCB* Queue::get(){

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(lastEl!=NULL){
	if(lastEl!=firstEl){
	queueElement* pom=firstEl;                      
	while(pom->next!=lastEl)pom=pom->next;
	PCB* toReturn=(lastEl->PCBpointer); 
	queueElement* pom2=lastEl;
	lastEl->PCBpointer=NULL;
	lastEl=pom;
	br--;
	pom->next=NULL;
	delete pom2;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

	return toReturn;
	}
	else{
		PCB* toReturn=(lastEl->PCBpointer); 
		queueElement* pom=lastEl;
		br--;
		lastEl->PCBpointer=NULL;
		lastEl=firstEl=NULL;
		delete pom;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

	return toReturn;
	}
	}
	
	else{

	#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

		return NULL;
	}
}

queueElement::queueElement(PCB* p){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	PCBpointer=p;
	next=NULL;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


