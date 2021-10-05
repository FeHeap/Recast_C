#include <stdio.h>
#include <stdlib.h>
#define VIDEO_BASE 0xB8000
#define PORT 0xB0000000

int main(){
	int num = 8;
	int *pi = (int*)num;

	
	pi = &num;
	printf("Before: %p\n", pi);
	int tmp = (int)pi;
	pi = (int*)tmp;
	printf("After: %p\n\n", pi);


//	int *video = (int*)VIDEO_BASE;
//	*video = 'A';


	unsigned int volatile * const port = (unsigned int *)PORT;
//	*port = 0x0BF4;
//	int value = *port;


	int n = 0x12345678;
	char *pc = (char*)&n;
	int i;
	for(i = 0; i < 4; i++){
		printf("%p: %02x\n", pc, (unsigned char)*pc++);
	}
	printf("\n");
	

	system("PAUSE");
	return 0;
}

