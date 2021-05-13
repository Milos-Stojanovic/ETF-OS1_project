#include "kernel.h"

/*
	Test: brojac sekundi
*/

unsigned t=18;
unsigned seconds = 5;

void secondPast()
{
	if(seconds)
		syncPrintf("%d\n",seconds);
	seconds--;
}

void tick()
{
	t--;
	if(t==0){
		t = 18;
		secondPast();
	}
}



int userMain(int argc, char** argv)
{
	syncPrintf("Starting test\n");
	while(seconds);
	syncPrintf("Test ends\n");
	return 0;
}
