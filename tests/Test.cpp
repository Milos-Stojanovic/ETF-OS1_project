/*
 * Test.cpp
 *
 *  Created on: 01.09.2020.
 *      Author: OS1
 */


#include "system.h"
#include "semaphor.h"
#include "thread.h"
#include <stdlib.h>
#include <time.h>

Semaphore sem(1);

class TestThread : public Thread{
public:
	TestThread(char c):Thread(1024,1){
		this->myPCB->keyType=c;
		sem2=new Semaphore(0);
	}

protected:
	void run(){
		//for(int j=0; j<5; j++){
		sem.funkcija1(this->myPCB->keyType);
		sem2->wait(20);
		sem.funkcija2();
		//}
	}

private:
	Semaphore* sem2;
};



int userMain(int argc, char* argv[]){
	TestThread* niz[10];
	srand(time(NULL));
	for(int i=0;i<10; i++){
		int a=rand()%2;
		char c;
		if(a==1)
			c='r';
		else
			c='w';

		niz[i]=new TestThread(c);
		niz[i]->start();
	}

	for(int j=0; j<10; j++){
		niz[j]->waitToComplete();
	}

	return 0;
}




