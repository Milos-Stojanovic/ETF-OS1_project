/*
 * SYSTEM.h
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */



#ifndef SYSTEM_H_
#define SYSTEM_H_


#include "idle.h"
#include"PCB.h"
#include "thread.h"
#include "KernelSe.h"
#include "iostream.h"


#define lock() {asm{pushf; cli; }}  //lock()
#define unlock(){ asm {popf;} }       //unlock()

class Thread;       
class Queue;



class ListaSemafora{
public:
	struct Element{
		Element* next;
		KernelSem* pok;
		Element(KernelSem* k){next=NULL; pok=k; }
		~Element();
	};

	 Element* prvi,*poslednji, *pom11;
	 queueElement* pom21, *pom3;
	ListaSemafora(){ prvi=NULL; poslednji=NULL; pom11=NULL; pom21=pom3=NULL;}
	~ListaSemafora();
	void dodaj(KernelSem* k);
	void izbaci(KernelSem* k);
};



class system {
public:
	static void load();
	static void unload();
	static void dispatch();
	static  PCB* running;
	static  IdleThread* idle;
	static Queue* q;  //za getThreadById


	static ListaSemafora::Element* pom1;
	static PCB* novi; 
	static volatile int flag;
	static  ListaSemafora* Lista;



	static  Thread* starter;

	static void wrapper(); // f-ja iz koje se poziva funkcija run (na taj nacin se obezbedjuje polimorfizam)

private:
	static void interrupt myRoutine(...);
	static volatile void interrupt(*oldRoutine)(...);
	static volatile int disp;



};

#endif /* SYSTEM_H_ */
