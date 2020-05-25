#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>
#include <stdlib.h> //su dung rand
#include <pthread.h>

unsigned long long totalPoints;
unsigned long long totalPointsIn;
#define NUM_THREADS 10
sem_t lock;

void *generate(){
	unsigned int core = rand();
	unsigned long long number = totalPoints/NUM_THREADS;
	//printf("number: %i\n",number);						//
	for(int k=1; k< number; k++){
		double x = -1 + rand_r(&core) / ((double)RAND_MAX + 1) * (1- -1) ;
		double y = -1 + rand_r(&core) / ((double)RAND_MAX + 1) * (1- -1) ;
      		//printf("Codinate (%f,%f)\n",x,y);				//
		sem_wait(&lock);
        	if( x*x + y*y <= 1 ) ++totalPointsIn;
		sem_post(&lock);
	}

	pthread_exit(0);
}

int main( int argc, char*argv[]){
	if(argc != 2) return -1;
	if(atoi(argv[1]) < 0 ) return -1;
	
	clock_t start, end;							//
	start = clock();							//

	srand( (int)time(0) );	//sinh so ngau nhien dua tren time

	sem_init(&lock, 0, 1);
	totalPoints = atoi(argv[1]);
	pthread_t threads[NUM_THREADS];

	for(int i =0; i< NUM_THREADS; ++i){
		pthread_create(&threads[i],NULL, generate,NULL);
	}
	for(int j =0; j< NUM_THREADS; ++j){
		pthread_join(threads[j],NULL);	//segmentation fault ????
	}

	sem_destroy(&lock);	

	double pi = 4.0 * totalPointsIn / totalPoints;
	printf("Pi = %f\n",pi);

	end = clock();									//
	printf("Time: %.5lf(s)\n", ((double)(end-start))/CLOCKS_PER_SEC);			//
	pthread_exit(0);
	return 0;
}
