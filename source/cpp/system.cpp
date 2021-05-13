/*
 * SYSTEM.cpp
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */


#include"system.h"
#include <dos.h>
#include<stdlib.h>
#include "queue.h"
#include "SCHEDULE.H"
#include "stdio.h"
#include "thread.h"
#include "PCB.h"




volatile void interrupt (*system::oldRoutine)(...)=NULL;
volatile int system::disp=0;


 Thread* system::starter=NULL;
 PCB* system::running=NULL;
 IdleThread* system::idle=NULL;
Queue* system::q=NULL;
 ListaSemafora* system::Lista=NULL; 

ListaSemafora::Element* system::pom1=NULL;
volatile int system::flag=0;
PCB* system::novi=NULL; 


void ListaSemafora::izbaci(KernelSem* k){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
ListaSemafora::Element* hej=prvi;
	ListaSemafora::Element* pom=NULL;
		while(hej->pok!=k){
			pom=hej;
			hej=hej->next;
		}

		hej->pok=NULL;

		if(pom==NULL){
			if(hej->next!=NULL){
				prvi=hej->next;
			}
			else{
				prvi=NULL;
				poslednji=NULL;
			}
		}
		else{
			if(hej->next==NULL){
				poslednji=pom;
				pom->next=NULL;
			}
			else{
				pom->next=hej->next;
			}
		}
			delete hej;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}



void system::load(){ //*-*



#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

#ifndef BCC_BLOCK_IGNORE

	oldRoutine=getvect(0x08);
	setvect(0x08,myRoutine);
#endif

	//ovde treba i main() pretvoriti u legitimnu nit (kao da je iz klase Thread())
	//i pravi se FIFO red za getThreadById metodu



	q=new Queue();  //novi FIFO red

	starter=new Thread(1000 ,1);
	starter->myPCB->ThreadStatus=READY;
	 

	q->put(starter->myPCB); //dodajem pocetnu nit u taj red
	running=starter->myPCB;

	//pravim i listu semafora (inicijalno praznu :( )
	Lista=new ListaSemafora();

	//napraviti idle nit
	idle=new IdleThread();

	idle->start();
#ifndef BCC_BLOCK_IGNORE

	unlock();
#endif
} //*-*

void system::unload(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(running != starter->myPCB ){
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
		return;
	}



#ifndef BCC_BLOCK_IGNORE
	setvect(0x08,oldRoutine);

	delete Lista;
	delete idle;
	delete starter;
	delete q;

	unlock();
#endif
}


void system::dispatch(){

#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	disp=1;
	myRoutine();
	disp=0;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

ListaSemafora::~ListaSemafora(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
		while(prvi!=NULL){

			Element* pom=prvi;
			prvi=prvi->next;
			delete pom;
		}
		poslednji=NULL;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	}

void ListaSemafora::dodaj(KernelSem* k) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	ListaSemafora::Element* asd = new ListaSemafora::Element(k);
	if (prvi == NULL) { //svi semafori idu u jednu listu
		prvi = asd;
	}
	else {
		asd->next = prvi;
		prvi = asd;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


ListaSemafora::Element::~Element(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}


void interrupt system::myRoutine(...){
	static volatile unsigned int tss, tsp;
	static  PCB* nextThread=NULL; 


	  //poziva se stara prekidna rutina ukoliko nije zatrazena promena konteksta
	if(!disp){
		tick();
			(*oldRoutine)();
	}

	//deblokiraju se sve niti sa semafora ako im je isteklo vreme blokiranja
			if(!disp){
				

				pom1=Lista->prvi;
				while(pom1!=NULL){
					flag=pom1->pok->waitingQueue->br;
					while(flag>0){

						novi=pom1->pok->waitingQueue->get();
						novi->myTime--;
						if(novi->myTime==0){
							pom1->pok->val++;
							novi->ThreadStatus=READY;
							novi->myReturnValue=0;
							Scheduler::put(novi);

						}
						else{
							pom1->pok->waitingQueue->put(novi);
						}

						--flag;
					}

					pom1=pom1->next;
				}

		   }

	if((!disp) && (running->timeSlice!=0)) //zbog niti koja moze trajati beskonacno dugo
		running->myTimerCounter+=1;



	if(!disp && (((running->myTimerCounter)<(int)(running->timeSlice))||(running->timeSlice==0))){

		return;
	}
		//krece promena konteksta
	disp=0;




	if((running!=idle->myPCB)&&(running->ThreadStatus==READY))
			Scheduler::put(running);

	nextThread=Scheduler::get();
	if(nextThread==NULL) nextThread=idle->myPCB;



	    //privremeno se sacuvaju trenutne vrednosti ss i sp u tss i tsp
#ifndef BCC_BLOCK_IGNORE
	asm{

		mov tsp, sp

		mov tss, ss
	}
#endif
	// tss i tsp se sacuvaju u PCB strukturi
	running->sp=tsp;
	running->ss=tss;




	running=nextThread;

	running->myTimerCounter=0; //restartovanje tajmera za nit izabranu iz Scheduler-a

	tsp=running->sp;
	tss=running->ss;

	//restaurira se sadrzaj steka
#ifndef BCC_BLOCK_IGNORE
	asm{

		mov sp, tsp

		mov ss, tss
	}
#endif




return;


}



void system::wrapper(){




	//najpre pozovi funkciju run
	running->myThread->run();

	//nakon ovoga treba postaviti niti status ZAVRSENA (da ne bi mogla da se vrati u Scheduler i nastavi sa izvrsavanjem)
	//i pre nego sto se nit zavrsi, mora da saceka da se sva njena deca zavrse
#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif
	running->ThreadStatus=FINISHED;



	PCB* pom;
	while(running->waitQueue->firstEl!=NULL){
		pom=running->waitQueue->get();
		pom->ThreadStatus=READY;
		Scheduler::put(pom);

	}

	dispatch();// na kraju se uradi promena konteksta - izvrsava se neka nit koja nije FINISHED
#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}






