#include <stdio.h>
#include <stdlib.h>

//strict aliasing
typedef union _conversion1{
	float fNum;
	unsigned int uiNum;
}Conversion1;

int isPositive1(float number){
	Conversion1 conversion = { .fNum = number };
	return (conversion.uiNum & 0x80000000) == 0;
}

//break strict aliasing
typedef union _conversion2{
	float *fNum;
	unsigned int *uiNum;
}Conversion2;

int isPositive2(float number){
	Conversion2 conversion;
	conversion.fNum = &number;
	return (*conversion.uiNum & 0x80000000) == 0;
}

//break strict aliasing
int isPositive3(float number){
	unsigned int *ptrValue = (unsigned int*)&number;
	return (*ptrValue & 0x80000000) == 0;
}

typedef struct _person{
	char *firstName;
	char *lastName;
	unsigned int age;
}Person;

//typedef struct _employee{
//	char *firstName;
//	char *lastName;
//	unsigned int age;
//}Employee;

typedef Person Employee;

void add(int size, double * restrict arr1, double * restrict arr2){
	int i;
	for(i = 0; i < size; i++){
		arr1[i] += arr2[i];
	}
}

int main(){
	int num = 5;
	int *p1 = &num;
	int *p2 = &num;
	
	
	float number = 3.25f;
	unsigned int *ptrValue = (unsigned int*)&number;
	unsigned int result = (*ptrValue & 0x80000000) == 0;
	
	
	Person* person;
	Employee* employee;
	
	double vector1[] = { 1.1, 2.2, 3.3, 4.4 };
	double vector2[] = { 1.1, 2.2, 3.3, 4.4 };
	add(4, vector1, vector2);
	double *vector3 = vector1;
//	add(4, vector1, vector3);
//	add(4, vector1, vector1);

	system("PAUSE");
	return 0;
}

