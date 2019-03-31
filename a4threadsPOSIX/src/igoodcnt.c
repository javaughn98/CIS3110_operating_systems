#include "../include/header.h"



int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;
	int numOfIncrements = 0;
	
	
	sem_init(&sem_main, 0, 1);

	// validate arguments
	if(argc < 2 || argc > 2) {
		fprintf(stderr, "usage: %s InvalidNumberofCommand-LineArguments\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else if(!isNum(argv[1])) {
		fprintf(stderr, "usage: %s ArgumentEnteredShouldBeANumber\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else if(atoi(argv[1]) != NITER) {
		fprintf(stderr, "usage: %s NumOfTimesEachThreadIncrements\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else {
		numOfIncrements = atoi(argv[1]);
	}
	

	// implementation


	// Parsing the arguments passed to your C program
	// Including the number of times that each thread increments
	// the shared count cnt
	// For example, NITER = 20000;
	// Display the number of times (entered by the user) that each thread
	// increments the shared count cnt
// PLEASE DO NOT remove or modify the following code 
	printf("2*NITER is [%d]\n", 2*NITER);
// End of code section 

	// creating Thread 1
	if(pthread_create(&tid1, NULL, Count, NULL) != 0)
	{
		fprintf(stderr, "usage: %s ErrorCreatingThread1\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// creating Thread 2
	if(pthread_create(&tid2, NULL, Count, NULL) != 0)
	{
		fprintf(stderr, "usage: %s ErrorCreatingThread2\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// test if the thread becomes deadlock 
	if(pthread_join(tid1, NULL) != 0)	/* wait for the thread 1 to finish */
	{
		fprintf(stderr, "usage: %s ErrorJoiningThread1\n", argv[0]);
		exit(1);
	}

	if(pthread_join(tid2, NULL) != 0)        /* wait for the thread 2 to finish */
	{
		fprintf(stderr, "usage: %s ErrorJoiningThread2\n", argv[0]);
		exit(1);
	}

        // Display the value of count cnt
// PLEASE DO NOT remove or modify the following code
	printf("\nCounter is [%d]\n", cnt);
	if (cnt == 2 * NITER) 
		printf("\nPASS\n");
	else
		printf("\nFAIL\n");
// End of code section
	sem_destroy(&sem_main);
	pthread_exit(NULL);
}



void *Count(void *a) {
   	int i, tmp;

	sem_wait(&sem_main);
    	for(i = 0; i < NITER; i++)
    	{
        	tmp = cnt;      /* copy the global cnt locally */
        	tmp = tmp+1;    /* increment the local copy */
        	cnt = tmp;      /* store the local value into the global cnt */ 
   	}
	
	sem_post(&sem_main);
		

}

// test if the arguments entered are all numbers
bool isNum(char *array) {
	int i =  0;

	for(i = 0; i < strlen(array); i++) {
		if(isdigit(array[i]) == false) {
			return false; // if argument contains characters that are not numbers
		}	
	} 
	
	return true;
}



