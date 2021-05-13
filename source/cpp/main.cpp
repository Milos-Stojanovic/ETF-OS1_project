/*
 * main.cpp
 *
 *  Created on: 18.04.2020.
 *      Author: OS1
 */

#include "system.h"


extern int userMain(int argc, char* argv[]);
int main(int argc, char* argv[]){
	system::load();//Promena prekidne rutine koja se izvrsava na svakih 55ms


	int value=userMain(argc, argv);

	system::unload();

	return value;
}



