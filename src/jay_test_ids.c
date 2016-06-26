#include "dynamixel.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <pthread.h>

#define MAXIDS 4

#define TORQUE_ENABLE 1
#define TORQUE_DISABLE 0

//global variables
pthread_mutex_t lock;
int pos;
int jointSocket[4];
//

void signal_callback_handler(int signum)
{
  printf("Caught signal %d\n", signum);
  //dxl_terminate();
  exit(signum);
}

void *writeThread(void*);
void *readThread(void*);

int main(int argc, char *argv[]){

  pthread_t rT, wT;
  int socket[4] = {4, 5, 6, 11};
  int i;

  int id[4] = {1,2,3,4};

  for(i = 0; i<argc-1; i++){
    if(dxl_initialize(&jointSocket[i], socket[i], 8) == 0){
      printf("Failed to open port /dev/ttyS%d\n", socket[i]);
      return -1;
    }
  }

  for(i = 0; i <4 ; i++){
    printf("%d\n",jointSocket[i] );
  }

  for(i = 1; i<MAXIDS; i++){
    dxl_write_word(jointSocket[i], id[i], 0x18, TORQUE_ENABLE);
  }

  pos = -1;
  //FIXME: Não faz o que deveria fazer. hue? so, do i always have to type like this?asdasda
  pthread_create(&rT, NULL, readThread, NULL);
  pthread_create(&wT, NULL, writeThread, NULL);

  sleep(10);

  return EXIT_SUCCESS;
}

void *writeThread(void* n){
  pthread_detach(pthread_self());
  while(1){
    pthread_mutex_lock(&lock);
    dxl_write_word(jointSocket[1], 3, 0x1E, pos);
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

void *readThread(void *n){
  pthread_detach(pthread_self());
  while(1){
    pthread_mutex_lock(&lock);
    pos = dxl_read_word(jointSocket[0], 1, 0x24);
    if(dxl_get_result() != 1)
      pos = -1;
    pthread_mutex_unlock(&lock);
    printf("%d\n", pos);
  }
  return NULL;
}
