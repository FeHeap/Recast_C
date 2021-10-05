#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 4

typedef struct{
	double *vectorA;
	double *vectorB;
	double sum;
	int length;
}VectorInfo;

typedef struct{
	VectorInfo *info;
	int beginningIndex;
}Product;

pthread_mutex_t mutexSum;

void dotProduct(void *prod){
	Product *product = (Product*)prod;
	VectorInfo *vectorInfo = product->info;
	int beginningIndex = product->beginningIndex;
	int endingIndex = beginningIndex + vectorInfo->length;
	double total = 0;
	
	int i;
	for(i = beginningIndex; i < endingIndex; i++){
		total += (vectorInfo->vectorA[i] * vectorInfo->vectorB[i]);
	}
	
	pthread_mutex_lock(&mutexSum);
	vectorInfo->sum += total;
	pthread_mutex_unlock(&mutexSum);
	
	pthread_exit((void*)0);
} 

void threadExample(){
	VectorInfo vectorInfo;
	double vectorA[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0 };
	double vectorB[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0 };
	
	double sum;
	
	vectorInfo.vectorA = vectorA;
	vectorInfo.vectorB = vectorB;
	vectorInfo.length = 4;
	
	pthread_t threads[NUM_THREADS];
	void *status;
	pthread_attr_t attr;
	
	pthread_mutex_init(&mutexSum, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	int returnValue;
	int threadNumber;
	
	for(threadNumber = 0; threadNumber < NUM_THREADS; threadNumber++){
		Product *product = (Product*)malloc(sizeof(Product));
		product->beginningIndex = threadNumber * 4;
		product->info = &vectorInfo;
		returnValue = pthread_create(&threads[threadNumber], &attr, dotProduct, (void*)(product));
		
		if(returnValue){
			printf("ERROR: Unable to create thread: %d\n", returnValue);
			exit(-1);
		} 
	}
	
	pthread_attr_destroy(&attr);
	int i;
	for(i = 0; i < NUM_THREADS; i++){
		pthread_join(threads[i], &status);
	}
	
	pthread_mutex_destroy(&mutexSum);
	printf("Dot Product sum: %lf\n", vectorInfo.sum);
	pthread_exit(NULL);
}

typedef struct _factorialData{
	int number;
	int result;
	void (*callBack)(struct _factorialData*);
}FactorialData;

void factorial(void *args){
	FactorialData *factorialData = (FactorialData*)args;
	void (*callBack)(FactorialData*);
	
	int number = factorialData->number;
	callBack = factorialData->callBack;
	
	int num = 1;
	int i;
	for(i = 1; i <= number; i++){
	 	num *= i;
	}
	
	factorialData->result = num;
	callBack(factorialData);
	
	pthread_exit(NULL); 
}

void startThread(FactorialData *data){
	pthread_t thread_id;
	int thread = pthread_create(&thread_id, NULL, factorial, (void*)data);
}

void callBackFunction(FactorialData *factorialData){
	printf("Factorial is %d\n", factorialData->result);
}

int main(){
	FactorialData *data = (FactorialData*)malloc(sizeof(FactorialData));
	if(!data){
		printf("Failed to allocate memory\n");
		return;
	}
	
	data->number = 5;
	data->callBack = callBackFunction;
	
	startThread(data);
	
	Sleep(2000);

	
	threadExample();

	system("PAUSE");
	return 0;
}

