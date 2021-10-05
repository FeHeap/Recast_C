#include <stdio.h>
#include <stdlib.h>

#define safeFree(p); saferFree((void**)&(p));

float average(int*, int);
void swapWithPointers(int*, int*);
void swap(int, int);
void passingAddressOfConstants_1(const int*, int*);
void passingAddressOfConstants_2(const int*, int*);
int* allocateArray_1(int, int);
void allocateArray_2(int*, int, int);
void allocateArray_3(int**, int, int);
void allocateArray_4(int*, int, int);
void saferFree(void**);

int (*fptr1)(int);
int square(int num){
	return num * num;
}

typedef int (*funcptr)(int);


int add(int num1, int num2){
	return num1 + num2;
}

int sub(int num1, int num2){
	return num1 - num2;
}

typedef int (*fptrOperation)(int, int);
int compute(fptrOperation operation, int num1, int num2){
	return operation(num1, num2);
}

fptrOperation select(char opcode){
	switch (opcode){
		case '+': return add; 
		case '-': return sub;
	}
}

int evaluate(char opcode, int num1, int num2){
	fptrOperation operation = select(opcode);
	return operation(num1, num2);
}

typedef int (*fptroperation)(int, int);
fptroperation operations[128] = {NULL};
void initializeOperationsArray(){
	operations['+'] = add;
	operations['-'] = sub;
}

int evaluateArray(char opcode, int num1, int num2){
	fptroperation operation;
	operation = operations[opcode];
	return operation(num1, num2);	
}

typedef int (*fptrToSingleInt)(int);
typedef int (*fptrToTwoInts)(int, int);

typedef void (*fptrBase)();

int main(){
	int a[] = {1, 2, 3};
	int b = 3;
	average(a,b);
	printf("\n\n");
	
	int n1 = 5, n2 = 10;
	printf("n1 = %d, n2 = %d\n", n1, n2);	//n1 = 5, n2 = 10
	swap(n1, n2);							//Swapping...
	printf("n1 = %d, n2 = %d\n", n1, n2);	//n1 = 5, n2 = 10
	swapWithPointers(&n1, &n2);				//Swapping With Pointers...
	printf("n1 = %d, n2 = %d\n", n1, n2);	//n1 = 10, n2 = 5
	printf("\n\n");
	
	const int limit = 100;
	int result = 5;
	printf("limit = %d, result = %d\n", limit, result);	//limit = 100, result = 5
	passingAddressOfConstants_1(&limit, &result);
	printf("limit = %d, result = %d\n", limit, result);	//limit = 100, result = 100
	//passingAddressOfConstants_2(&limit, &result);	[Error] assignment of read-only location '*num1'
	//passingAddressOfConstants_2(&limit, &limit);	[Note] expected 'int *' but argument is of type 'const int *'
	//passingAddressOfConstants_1(&23, &23);	[Error] lvalue required as unary '&' operand
	printf("\n\n");
	
	int* vector = allocateArray_1(5, 45);
	int i;
	for(i = 0; i < 5; i++){
		printf("%d\n",vector[i]);	//45
	}
	free(vector);
	vector = NULL;
	printf("\n");
	
	vector = (int*)malloc(5 * sizeof(int));
	allocateArray_2(vector, 5, 35);
	for(i = 0; i < 5; i++){
		printf("%d\n",vector[i]);	//35
	}
	free(vector);
	vector = NULL;
	printf("\n");
	
	printf("%p\n", vector);	//0000000000000000
	allocateArray_3(&vector, 5, 25);
	printf("%p\n", vector);	//00000000007113E0
	free(vector);
	
//	memory leak!
//	vector = NULL;
//	printf("%p\n", vector);	//0000000000000000
//	allocateArray_4(&vector, 5, 15);
//	printf("%p\n", vector);	//0000000000000000

	int *pi;
	pi = (int*)malloc(sizeof(int));
	*pi = 5;
	printf("Before: %p\n",pi);	//Before: 00000000007113E0
	safeFree(pi);				//SAFER FREE
	printf("After: %p\n",pi);	//After: 0000000000000000
	safeFree(pi);				//SAFER FREE
	printf("\n\n");
	
	int n = 5;
	fptr1 = square;
	printf("%d squared is %d\n", n, fptr1(n));	//5 squared is 25
	
	funcptr fptr2;
	fptr2 = square;
	printf("%d squared is %d\n", n, fptr2(n));	//5 squared is 25
	
	printf("%d\n",compute(add, 5, 6));	//11
	printf("%d\n",compute(sub, 5, 6));	//-1
	
	printf("%d\n", evaluate('+', 5, 6));	//11
	printf("%d\n", evaluate('-', 5, 6));	//-1
	
	initializeOperationsArray();
	printf("%d\n", evaluateArray('+', 5, 6));	//11
	printf("%d\n", evaluateArray('-', 5, 6));	//-1
	printf("\n\n");
	
	fptrOperation fptr3 = add;
	if(fptr3 == add){
		printf("fptr3 points to add function\n");	//fptr3 points to add function
	}
	else{
		printf("fptr3 does not point to add function\n");	
	}
	printf("\n\n");
	
	fptrToTwoInts fptrFirst = add;
	fptrToSingleInt fptrSecond = (fptrToSingleInt)fptrFirst;
	fptrFirst = (fptrToTwoInts)fptrSecond;
	printf("%d\n", fptrFirst(5, 6));	//11
	
	fptrBase basePointer;
	fptrFirst = add;
	basePointer = (fptrToSingleInt)fptrFirst;
	fptrFirst = (fptrToTwoInts)basePointer;
	printf("%d\n", fptrFirst(5, 6));	//11
	
	system("PAUSE");
	return 0;
}


float average(int *arr, int size){
	int sum;
	printf("arr: %p\n", &arr);		//000000000062FDB0
	printf("size: %p\n", &size);	//000000000062FDB8
	printf("sum: %p\n", &sum);		//000000000062FD9C
	
	int i;	//000000000062FD98
	printf("i: %p\n", &i);
	for(i = 0; i < size; i++){
		sum += arr[i];
	}
	
	return (sum * 1.0f) / size;
}

void swap(int num1, int num2){
	printf("Swapping...\n");
	int temp;
	temp = num1;
	num1 = num2;
	num2 = temp;
} 

void swapWithPointers(int* pnum1, int* pnum2){
	printf("Swapping With Pointers...\n");
	int temp;
	temp = *pnum1;
	*pnum1 = *pnum2;
	*pnum2 = temp;
}

void passingAddressOfConstants_1(const int *num1, int *num2){
	*num2 = *num1;
}

/*
void passingAddressOfConstants_2(const int *num1, int *num2){
	*num1 = 100;	//[Error] assignment of read-only location '*num1'
	*num2 = 200;
}
*/

int* allocateArray_1(int size, int value){
	int *arr = (int*)malloc(size * sizeof(int));
	int i;
	for(i = 0; i < size; i++){
		arr[i] = value;
	}
	return arr;
}

void allocateArray_2(int *arr, int size, int value){
	if(arr != NULL){
		int i;
		for(i = 0; i < size; i++){
			arr[i] = value;
		}
	}
}

void allocateArray_3(int **arr, int size, int value){
	*arr = (int*)malloc(size * sizeof(int));
	if(*arr != NULL){
		int i;
		for(i = 0; i < size; i++){
			*(*arr+i) = value;
		}
	}
}

void allocateArray_4(int *arr, int size, int value){	//memory leak
	arr = (int*)malloc(size * sizeof(int));
	if(*arr != NULL){
		int i;
		for(i = 0; i < size; i++){
			arr[i] = value;
		}
	}
}

void saferFree(void **pp){
	printf("SAFER FREE\n");
	if(pp != NULL && *pp != NULL){
		free(*pp);
		*pp = NULL;
	}
}
