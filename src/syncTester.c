#include "dynamixel.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#define TORQUE_ENABLE 1
#define TORQUE_DISABLE 0


void signal_callback_handler(int signum)
{
	printf("Caught signal %d\n", signum);
	//dxl_terminate();
	exit(signum);
}

int main(int argc, char *argv[]){

	int socket = 0;
	int i;
	int jointSocket = 0;

	int values[5];
	//FIXME: put the correct ID's here;
	int ids[5] = {25,28,29,34,33 };


	if((jointSocket = dxl_initialize(socket, 8)) == 0){
		printf("Failed to open port /dev/ttyUSB%d\n", socket);
		return -1;
	}

	printf("Opened joint socket: %d\n", socket);

	for(i = 0; i<5; i++){
		dxl_write_word(jointSocket, ids[i], 0x18, TORQUE_ENABLE);
	}


	int j;
	/*
	   for(i = 0;i<2048;i++){
	   for(j = 0; j<5; j++)
	   values[j] = j*100 + i;	  
	   dxl_sync_write_word(jointSocket, 0x1E, ids, values, 6);
	   usleep(100000);
	   }
	 */

	for(i = 0; i<200; i++){
		for(j = 0; j<5; j++){
			dxl_write_word(jointSocket, ids[j], 0x1E, j*100+i);
		}
		usleep(1000);
	}


	dxl_terminate(jointSocket);
	return EXIT_SUCCESS;
}

