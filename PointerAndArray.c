#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getLine(void);
char* trim(char*);
void displayArray(int[], int);
void display2DArray(int[][5], int);
void display2DArrayUnknownSize(int*, int, int);
void display3DArray(int [][2][4], int);

int main(){
	int vector[5] = {1, 2, 3, 4, 5};
	printf("%d\n\n", sizeof(vector)/sizeof(int));
	
	int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
	int i;
	for(i = 0; i < 2; i++){
		printf("&matrix[%d]: %p sizeof(matrix[%d]): %d\n", i, &matrix[i], i, sizeof(matrix[i]));
	}
	printf("\n\n");
	
	int Vector[5] = {1, 2, 3, 4, 5};
	int *pV;
	printf("%p\n", vector);
	printf("%p\n", &vector[0]);
	printf("%p\n", &vector);
	pV = Vector;
	int value = 3;
	for(i = 0; i < 5; i++){
		*pV++ *= value;
	}
	for(i = 0; i < 5; i++){
		printf("%d ", Vector[i]);
	}
	printf("\n\n");
	
	int *pv = (int*)malloc(5 * sizeof(int));
	for(i = 0; i < 5; i++){
		pv[i] = i + 1;
	}
	for(i = 0; i < 5; i++){
		printf("%d ", pv[i]);
	}
	printf("\n");
	for(i = 0; i < 5; i++){
		printf("%d ", *(pv + i));
	}
	printf("\n");
	free(pv);
	printf("\n");
	
	printf("%d\n\n",'\0');
	
	char *input;
	printf("input something:");	//Once upon a time there was a giant pumpkin
	input = getLine();
	input = trim(input);
	printf("input: %s\nstrlen(input): %zu\n", input, strlen(input));
	printf("sizeof(input): %zu\n\n", sizeof(input)); //not array
	free(input);
	
	int vecter_d[5] = {1, 2, 3, 4, 5};
	displayArray(vector, 5);
	printf("\n");
	
	int* arr[5];
	for(i = 0; i < 5; i++){
		arr[i] = (int*)malloc(sizeof(int));
		*arr[i] = i;
	}
	printf("arr:		%p\n",arr);
	printf("arr+1:		%p\n",arr+1);
	printf("*(arr+1):	%p\n",*(arr+1));
	printf("**(arr+1):	%p\n",**(arr+1));
	printf("*arr[0]:	%p\n",*arr[0]);
	printf("**arr:		%p\n",**arr);
	printf("arr[0][0]:	%p\n",arr[0][0]);
	printf("arr[3][0]:	%p\n\n",arr[3][0]);
	for(i = 0; i < 5; i++){
		free(arr[i]);
	}
	
	int Matrix[2][5] = {{1,2,3,4,5},{6,7,8,9,0}};
	for(i = 0; i < 2; i++){
		int j;
		for(j = 0; j < 5; j++){
			printf("matrix[%d][%d] Address: %p Value: %d\n", i, j, &Matrix[i][j], Matrix[i][j]);
		}
	}
	int (*pMatrix)[5] = Matrix;
	printf("Matrix:	   %p\n", Matrix);
	printf("Matrix + 1:%p\n", Matrix + 1);
	printf("sizeof(Matrix[0]):%zu.\n", sizeof(Matrix[0]));
	printf("%p	%d\n\n", Matrix[0]+1, *(Matrix[0]+1));
	
	int Matrix_2D[2][5] = {{1,2,3,4,5},{6,7,8,9,0}};
	display2DArray(Matrix_2D, 2);
	display2DArrayUnknownSize(&Matrix_2D[0][0], 2, 5);
	int Matrix_3D[3][2][4] = {
			{{1,2,3,4}, {5,6,7,8}},
			{{9,10,11,12}, {13,14,15,16}},
			{{17,18,19,20}, {21,22,23,24}}
	};
	display3DArray(Matrix_3D, 3);
	printf("\n");
	
	int rows = 2;
	int columns = 5;
	int **pmatrix = (int**)malloc(rows * sizeof(int*));
	for(i = 0; i < rows; i++){
		pmatrix[i] = (int*)malloc(columns * sizeof(int));
	}
	for(i = 0; i < rows; i++){
		free(pmatrix[i]);
	}
	free(pmatrix);
	
	rows = 2;
	columns = 5;
	int **Matrix_Contiguous_1 = (int**)malloc(rows * sizeof(int*));
	Matrix_Contiguous_1[0] = (int*)malloc(rows * columns * sizeof(int));
	for(i = 0; i < rows; i++)
		Matrix_Contiguous_1[i] = Matrix_Contiguous_1[0] + i * columns;
	free(Matrix_Contiguous_1[0]);
	free(Matrix_Contiguous_1);
	
	int *Matrix_Contiguous_2 = (int*)malloc(rows * columns * sizeof(int));
	for(i = 0; i < rows; i++){
		int j;
		for(j = 0; j < columns; j++){
			*(Matrix_Contiguous_2 + (i * columns) + j) = i*j;
		}
	}
	for(i = 0; i < rows; i++){
		int j;
		for(j = 0; j < columns; j++){
			printf("%d ",*(Matrix_Contiguous_2 + (i * columns) + j));
		}
		printf("\n");
	}
	free(Matrix_Contiguous_2);
	
	
	int (*(arr1[])) = {
		(int[]) {0, 1, 2},
		(int[]) {3, 4, 5},
		(int[]) {6, 7, 8}
	};
	for(i = 0; i < 3; i++){
		int j;
		for(j = 0; j < 3; j++){
			printf("arr1[%d][%d] Address: %p value: %d\n", i, j, &arr1[i][j], arr1[i][j]);
		}
		printf("\n");
	}
	
	int (*(arr2[])) = {
		(int[]) {0, 1, 2, 3},
		(int[]) {4, 5},
		(int[]) {6, 7, 8}
	};
	int row = 0;
	for(i = 0; i < 4; i++){
		printf("arr2[%d][%d] Address: %p value: %d\n", row, i, &arr2[row][i], arr2[row][i]);
	}
	printf("\n");
	row = 1;
	for(i = 0; i < 2; i++){
		printf("arr2[%d][%d] Address: %p value: %d\n", row, i, &arr2[row][i], arr2[row][i]);
	}
	printf("\n");
	row = 2;
	for(i = 0; i < 3; i++){
		printf("arr2[%d][%d] Address: %p value: %d\n", row, i, &arr2[row][i], arr2[row][i]);
	}
	printf("\n");
	
	system("PAUSE");
	return 0;
}

char* getLine(void){
	const size_t sizeIncrement = 10;
	char* buffer = (char*)malloc(sizeIncrement);
	char* currentPosition = buffer;
	size_t maximunLength = sizeIncrement;
	size_t length = 0;
	int character;
	
	if(currentPosition == NULL){ return NULL; }
	
	while(1){
		character = fgetc(stdin);
		if(character == '\n') { break; }
		
		if(++length >= maximunLength){
			char *newBuffer = realloc(buffer, maximunLength += sizeIncrement);
			
			if(newBuffer == NULL){
				free(buffer);
				return NULL;
			}
			
			currentPosition = newBuffer + (currentPosition - buffer);
			buffer = newBuffer;
		}
		
		*currentPosition++ = character;
	}
	
	*currentPosition = '\0';
	
	
	return buffer;
}

char* trim(char* phrase){
	char* old = phrase;
	char* new = phrase;
	
	while(*old == ' '){
		old++;
	}
	
	while(*old){
		*(new++) = *(old++);
	}
	
	*new = 0;
	return (char*)realloc(phrase, strlen(phrase) + 1);
}

void displayArray(int arr[], int size){
	int i;
	for(i = 0; i < size; i++){
		printf("%d\n", arr[i]);
	}
}

void display2DArray(int arr[][5], int rows){
	int i;
	for(i = 0; i < rows; i++){
		int j;
		for(j = 0; j < 5; j++){
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}
}

void display2DArrayUnknownSize(int *arr, int rows, int cols){
	int i;
	for(i = 0; i < rows; i++){
		int j;
		for(j = 0; j < cols; j++){
			printf("%d ", *(arr + (i * cols) + j));
		}
		printf("\n");
	}
}

void display3DArray(int (*arr)[2][4], int rows){
	int i;
	for(i = 0; i < rows; i++){
		int j;
		for(j = 0; j < 2; j++){
			printf("{");
			int k;
			for(k = 0; k < 4; k++){
				printf("%d ", arr[i][j][k]);
			}
			printf("}");
		}
		printf("\n");
	}
}
