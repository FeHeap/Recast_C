#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define PINT int*
#define SIZE 32

typedef int* pint;

void replace(char buffer[], char replacement, size_t size){
	size_t count = 0;
	while(buffer != NULL && count++ < size){
		*buffer = replacement;
		buffer++;
	}
}

typedef struct _employee{
	char name[10];
	int age;
}Employee;

typedef struct _item{
	int partNumber;
	int quantity;
	int binNumber;
}Item;

int getSystemStatus(){
	int status = 0;
	return status;
}

int (*fptrCompute)(int, int);
int add(int n1, int n2, int n3){
	return n1 + n2 + n3;
}

int main(int argc, char **argv){
	printf(argv[1]);
	printf("\n\n");
	
	
	int* ptr1, ptr2;
	printf("sizeof(ptr1) == %zu\n", sizeof(ptr1));
	printf("sizeof(ptr2) == %zu\n", sizeof(ptr2));
	PINT ptr3, ptr4;
	printf("sizeof(ptr3) == %zu\n", sizeof(ptr3));
	printf("sizeof(ptr4) == %zu\n", sizeof(ptr4));
	pint ptr5, ptr6;
	printf("sizeof(ptr5) == %zu\n", sizeof(ptr5));
	printf("sizeof(ptr6) == %zu\n", sizeof(ptr6));
	printf("\n");
	
//	int *pi;
//	printf("%d\n", *pi);

	int *pi = NULL;
	if(pi == NULL){
		
	}else{
		printf("%d\n",*pi);
	}
	
//	assert(pi != NULL);	Expression: pi != NULL
	int integer = 0;
	pi = &integer;
	assert(pi != NULL);	//Nothing happens
	
	float *vector = (float*)malloc(20 * sizeof(float));
	if(vector == NULL){
		printf("fail to malloc");
		exit(0);
	}
	else{
		int i;
		for(i = 0; i < 20; i++){
			vector[i] = 0;
		}
	}
	free(vector);
	
	
	int num;
	int *pi_1 = &num;
	int *pi_2;
	//*pi_2 = &num; Error!
	pi_2 = &num;
	
	
	char firstName[8] = "1234567";
	char middleName[8] = "1234567";
	char lastName[8] = "1234567";
	
	middleName[-10] = 'X';
	middleName[0] = 'X';
	middleName[16] = 'X';
	
	printf("%p %s\n", firstName, firstName);	//000000000062FDE0 X234567
	printf("%p %s\n", middleName, middleName);	//000000000062FDD0 X234567
	printf("%p %s\n", lastName, lastName);		//000000000062FDC0 123456X
	printf("\n");
	
	
	char name[8];
	strcpy(name, "Alexander");
	replace(name, '+', sizeof(name));
	printf("%s\n\n", name);
	
	
	int buffer[20];
	int *pbuffer = buffer;
	int i;
//	for(i = 0; i < sizeof(buffer); i++){
//		*(pbuffer++) = 0;
//	}
	

	int Num = 2147483647;
	int *Pi = &Num;
	short *Ps = (short*)Pi;
	printf("Pi: %p	Value(16): %x	Value(10): %d\n", Pi, *Pi, *Pi);										//Pi: 000000000062FD4C    Value(16): 7fffffff    Value(10): 2147483647
	printf("Ps: %p	Value(16): %hx		Value(10): %hd\n", Ps, (unsigned short)*Ps, (unsigned short)*Ps);	//Ps: 000000000062FD4C    Value(16): ffff        Value(10): -1
	printf("\n");
	
	char Name[SIZE];
	char *p = Name;
	if(name != NULL){
		if(p >= Name && p < Name+SIZE){
			
		}
		else{
			
		}
	}
	
	
	char first[8];
	int result;
	if(!(result = strcpy_s(first, sizeof(first), "Alexander"))){
		printf("first: %s\n\n", first);
	}
	else{
		printf("result: %d\n\n", result);
	}
	
	
	Employee employee;
	strcpy(employee.name, "Fe");
	employee.age = 20;
	char *ptr = employee.name;
	ptr += sizeof(employee.name);
	
	
	Item part = {12345, 35, 107};
	int *pi_ = &part.partNumber;
	printf("Part number: %d\n", *pi_);
	pi_++;
	printf("Quantity: %d\n", *pi_);
	pi_++;
	printf("Bin number: %d\n", *pi_);
	
	pi_ = &part.partNumber;
	printf("Part number: %d\n", *pi_);
	pi_ = &part.quantity;
	printf("Quantity: %d\n", *pi_);
	pi_ = &part.binNumber;
	printf("Bin number: %d\n", *pi_);
	
	printf("Part number: %d\n", part.partNumber);
	printf("Quantity: %d\n", part.quantity);
	printf("Bin number: %d\n", part.binNumber);
	printf("\n\n");
	
	
	if(getSystemStatus() == 0){
		printf("Status is 0\n");
	}
	else{
		printf("Status is not 0\n");
	}
	
	if(getSystemStatus == 0){
		printf("Status is 0\n");
	}
	else{
		printf("Status is not 0\n");
	}
	
	if(getSystemStatus){
		printf("Status is 0\n");
	}
	else{
		printf("Status is not 0\n");
	}
	
	if(getSystemStatus()){
		printf("Status is 0\n");
	}
	else{
		printf("Status is not 0\n");
	}
	
	
	fptrCompute = add;
	fptrCompute(2,5);
	
	char *n = (char*)malloc(sizeof(char));
	free(n);
//	free(n);
	n = NULL;
	
	
	char NAme[32];
	int userID;
	/*
	.
	.
	.
	*/
	memset(name, 0, sizeof(NAme));
	userID = 0;
	
	system("PAUSE");
	return 0;
}

