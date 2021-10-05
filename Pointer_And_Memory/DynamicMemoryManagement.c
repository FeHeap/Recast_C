#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#define RAII_VARIABLE(vartype,varname,initval,dtor) \
	void _dtor_ ## varname (vartype * v){ dtor(*v); } \
	vartype varname __attribute__((cleanup(_dtor_ ## varname))) = (initval)

void function();

void raiiExample();

int main(){

	#ifdef NUL
		if(NUL == '\0')
			printf("NUL == '\0'");
	#endif

	int a;
	printf("int a;\n");
	printf("sizeof(a) == %d\n\n",sizeof(a));
	
	int *b;
	printf("int *b;\n");
	printf("sizeof(b) == %d\n\n",sizeof(b));
	//printf("*b == %d\n\n",*b);  Error
	
	
	size_t c;
	printf("size_t c;\n");
	printf("sizeof(c) == %d\n",sizeof(c));
	c = -10;
	printf("c = -10;\n");
	printf("c == %zu\n",c);
	c = 10;
	printf("c = 10;\n");
	printf("c == %zu\n\n",c);
	
	//void *malloc(size_t);
	int *pi_malloc = (int*)malloc(1 * sizeof(int));
	memset(pi_malloc, 0, sizeof(int));
	printf("int *pi_malloc = (int*)malloc(1 * sizeof(int));\n");
	printf("memset(pi_malloc, 0, 1 * sizeof(int));\n");
	printf("*pi_malloc == %d\n\n",*pi_malloc);
	free(pi_malloc);
	
	
	//void *calloc(size_t numElements, size_t elementSize);
	int *pi_calloc = (int*)calloc(1, sizeof(int));
	printf("int *pi_calloc = (int*)calloc(1, sizeof(int));\n");
	printf("*pi_calloc == %d\n\n",*pi_calloc);
	free(pi_calloc);
	
	//void *realloc(void *ptr, size_t size);
	char *string1;
	char *string2;
	string1 = (char*)malloc(16);
	strcpy(string1, "01234567890AB");
	string2 = (char*)realloc(string1, 8);
	printf("char *string1;\n");
	printf("char *string2;\n");
	printf("string1 = (char*)malloc(16);\n");
	printf("strcpy(string1, \"01234567890AB\");\n");
	printf("string2 = (char*)realloc(string1, 8);\n");
	printf("string1 value: %p [%s]\n", string1, string1);
	printf("string2 value: %p [%s]\n\n", string2, string2);
	free(string1);
	//free(string2);
	
	char *string3;
	char *string4;
	string3 = (char*)malloc(16);
	strcpy(string3, "01234567890AB");
	string4 = (char*)realloc(string3, 64);
	printf("char *string3;\n");
	printf("char *string4;\n");
	printf("string3 = (char*)malloc(16);\n");
	printf("strcpy(string3, \"01234567890AB\");\n");
	printf("string4 = (char*)realloc(string3, 64);\n");
	printf("string3 value: %p [%s]\n", string3, string3);
	printf("string4 value: %p [%s]\n\n", string4, string4);
	free(string3);
	free(string4);
	
	raiiExample();
	
	system("PAUSE");
	return 0;
}

void function(){
	//static int *pi = (int*)malloc(sizeof(int)); bad initial
	static int *pi;
	pi = (int*)malloc(sizeof(int));
	free(pi);
}

void raiiExample(){
	RAII_VARIABLE(char*, name, (char*)malloc(32), free);
	strcpy(name, "RAII Example");
	printf("%s\n\n",name);
}



