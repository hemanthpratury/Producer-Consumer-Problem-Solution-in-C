#include "size1.h"
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>
//#define in 10
#define out 100
int input[MAX_NUMS]; //the input buffer that will be accesed by the producer and consumer 
int output[out];
int inC=0; //counter 
int no=0;
pthread_t tid,producer_thread[2],consumer_thread[3]; // these are the threads for 2producer and 2 consumer threads.    
pthread_mutex_t Lock; //mutex lock 
sem_t full,empty; //semaphores 
pthread_attr_t defattr; //thread attributes 
/*
This is the producer method, it generates a random number and inserts it 
in the common buffer
*/
void* producer(void* var){
	int item,i;
	
	for(i=0;i<MAX_NUMS;i++){
		
		item=(rand()%100)+1;
		sem_wait(&empty);
		pthread_mutex_lock(&Lock);
		if(inC<MAX_NUMS){
			printf("\nproduce: %d",item);
			input[inC]=item;
			inC++;
		}
		else{
		fprintf(stderr,"\nProducer error");
		}
	pthread_mutex_unlock(&Lock);
	sleep(1);
	sem_post(&full);
	
sleep(1);
	}
printf("\nproducer exiting\n");
//return 0;
}
/*
This is the consumer method and removes items from the 
commmon buffer 
*/
void* consumer(void* var){
int item,i;
	for(i=0;i<MAX_NUMS;i++){
		
		sem_wait(&full);
		pthread_mutex_lock(&Lock);
		
		
		
		if(inC>0){
			item=input[inC-1];
			inC--;
			printf("\nConsumer consumed: %d",item);
		}
		else{
		fprintf(stderr,"\nConsumer error");
		}	
	pthread_mutex_unlock(&Lock);
	sleep(1);
	sem_post(&empty);
	sleep(1);
	}
printf("\nConsumer exiting too\n");
//return 0;
}

int main()
{

pthread_mutex_init(&Lock,NULL); //initialize the mutex
sem_init(&full,0,0); //initialize the semeaphore full to 0
sem_init(&empty,0,MAX_NUMS); //initialize the semaphore empty to MAX_NUMS
pthread_attr_init(&defattr); //default attributes to the threads

pthread_create(&producer_thread[0],&defattr,producer,NULL); //creating producer thread 
pthread_create(&producer_thread[1],NULL,producer,NULL); //creating next producer 
pthread_create(&consumer_thread[0],&defattr,consumer,NULL); //creating consumer thread 
pthread_create(&consumer_thread[1],NULL,consumer,NULL); //creating next consumer 
//join the threads so that it can be suspended until current current thread finishes executing
 pthread_join(producer_thread[0],NULL);
pthread_join(producer_thread[1],NULL);
    pthread_join(consumer_thread[0],NULL);
 pthread_join(consumer_thread[1],NULL);

pthread_exit(NULL);
printf("\nEnding it \n");
exit(0);
}



