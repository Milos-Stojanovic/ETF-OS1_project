#include "thread.h"
#include "semaphor.h"
#include <iostream.h>
#include <stdlib.h>

int syncPrintf(const char *format, ...);

const int n = 10;
int count = 100;
Semaphore mutex(1);

int mutex_glb = 1;

void mutex_glb_wait(){
 sloop:asm{
	mov ax, 0
	xchg ax, mutex_glb
}
 if(_AX ==0){
	dispatch();
	asm jmp sloop;
 }
}


#define intLock mutex_glb_wait();
#define intUnlock mutex_glb = 1;


class BoundedBuffer {
public:

	BoundedBuffer (unsigned size);
	virtual ~BoundedBuffer ();
	int append (char);
	char take ();
	int  fullCount(){return itemAvailable.val();};    // potrebno consumeru

private:
	unsigned Size;
	Semaphore mutexa, mutext;
	Semaphore spaceAvailable, itemAvailable;
	char* buffer;
	int head, tail;

};

BoundedBuffer::BoundedBuffer (unsigned size) : Size(size),
	mutexa(1), mutext(1), spaceAvailable(size), itemAvailable(0),
	head(0), tail(0) {
		buffer = new char[size];
		if (!buffer) exit(1);
	}

BoundedBuffer::~BoundedBuffer(){
	intLock
	delete [] buffer;
	intUnlock
}

int BoundedBuffer::append (char d) {
	spaceAvailable.wait(0);
	mutexa.wait(0);
		buffer[tail] = d;
		tail = (tail+1)%Size;
	mutexa.signal();
	itemAvailable.signal();
	return 0;
}

char BoundedBuffer::take () {
	itemAvailable.wait(0);
	mutext.wait(0);
		char d = buffer[head];
		head = (head+1)%Size;
	mutext.signal();
	spaceAvailable.signal();
	return d;
}

BoundedBuffer buffer(n/2);

class Producer: public Thread
{
public:
	Semaphore sleep;
	Producer(): Thread(), sleep(0) {};
	~Producer()
	{
		waitToComplete();
	}

protected:
	void run()
	{
		char c;
		while(count>0)
		{
			sleep.wait(15);
			c = 65+(rand()%25);
			buffer.append(c);
			syncPrintf("%d. Producer %d puts '%c'\n",count,55,c);
			mutex.wait(0);
			count--;
			mutex.signal();
		}
	}
};

class Consumer: public Thread
{
public:
	Semaphore sleep;
	Consumer(): Thread(), sleep(0) {};
	~Consumer()
	{
		waitToComplete();
	}

protected:
	void run()
	{
		char c;
		while(count>0)
		{
			sleep.wait(15);
			c = buffer.take();
			syncPrintf("%d. Consumer %d gets '%c'\n",count,Thread::getRunningId(),c);
			mutex.wait(0);
			count--;
			mutex.signal();
		}
	}
};

void tick(){}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	Producer p;
	Consumer c;
	p.start();
	c.start();
	syncPrintf("Test ends.\n");
	return 0;
}

