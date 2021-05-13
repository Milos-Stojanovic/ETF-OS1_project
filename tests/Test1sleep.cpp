#include "thread.h"
#include "SEMAPHOR.H"
#include <iostream.h>
#include <stdio.h>

extern int syncPrintf (const char* format, ... );


void tick() {}

Semaphore sleep1(0);

class TestThread : public Thread
{
public:
	Semaphore sleep2;
	TestThread(int id): Thread(4096, 2), myID(id), sleep2(0) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:
	int myID;
	void run();

};

class WaitThread: public Thread
{
public:
	WaitThread(TestThread *t1, TestThread *t2): Thread()
	{
		t1_ = t1;
		t2_ = t2;
	};

	~WaitThread()
		{
			waitToComplete();
		}

protected:
	void run()
	{
		syncPrintf("Starting tests...\n");
		t1_->waitToComplete();
		syncPrintf("Test 1 completed!\n");
		t2_->waitToComplete();
		syncPrintf("Test 2 completed!\n");
	}
private:
	TestThread *t1_,*t2_;
};

void TestThread::run()
{
	syncPrintf("Thread %d adresa:%p: loop1 starts\n", getId(), this);

	//for(long i=0;i<8000;i++) {	for (long j = 0; j < 32000; j++);}
	sleep2.wait(20);

	syncPrintf("Thread %d: loop1 ends, dispatch\n",getId());

	dispatch();

	syncPrintf("Thread %d: loop2 starts\n", getId());

	//for (int k = 0; k < 8000; k++) { for (long j = 0; j < 32000; j++); }
	sleep2.wait(20);

	syncPrintf("Thread %d: loop2 ends\n", getId());


}
int userMain(int argc, char** argv)
{
	syncPrintf("User main starts\n");
	TestThread *t1 = new TestThread(1),
			*t2 = new TestThread(2),
			*t3 = new TestThread(3),
			*t4 = new TestThread(4),
				*t5 = new TestThread(5),
				*t6 = new TestThread(6);

	WaitThread *w = new WaitThread(t1,t2);
	WaitThread *w1 = new WaitThread(t5,t6);


	t1->start();
	t2->start();
	t3->start();
	t4->start();
	t5->start();
	t6->start();
	w->start();
	w1->start();

	for (long k=0; k<5; k++) {
		//for(long i=0;i<8000;i++) { for (long j = 0; j < 32000; j++);}
		sleep1.wait(20);

		syncPrintf("Usermain %d running!\n", Thread::getRunningId());
		Thread *t = Thread::getThreadById(3);
		syncPrintf("%p\n", t);
		dispatch();
	}

	delete w;
	delete w1;
	delete t1;
	delete t2;
	delete t3;
	delete t4;
	delete t5;
	delete t6;

	syncPrintf("User main ends\n");
	return 0;
}

