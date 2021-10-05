#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

size_t stringLength(const char* string);

char* format(char* buffer, size_t size, const char* name, size_t quantity, size_t weight){
	snprintf(buffer, size, "Item: %s  Quantity: %zu  Weight: %zu", name, quantity, weight);
	return buffer;
}

char* Format(char* buffer, size_t size, const char* name, size_t quantity, size_t weight){
	char *formatString = "Item: %s  Quantity: %u  Weight: %u";
	size_t formatStringLength = strlen(formatString) - 6;
	size_t nameLength = strlen(name);
	size_t length = formatStringLength + nameLength + 10 + 10 + 1;
	if(buffer == NULL) {
		buffer = (char*)malloc(length);
		size = length;
	}
	
	snprintf(buffer, size, formatString, name, quantity, weight);
	return buffer;
}

char* returnALiteral(int code){
	switch(code){
		case 100:
			return "Boston Processing Center";
		case 200:
			return "Denver Processing Center";
		case 300:
			return "Atlanta Processing Center";
		case 400:
			return "San Jose Processing Center";
	}
}

char* returnAStaticLiteral(int code, int subCode){
	static char *bpCenter = "Boston Processing Center";
	static char *dpCenter = "Denver Processing Center";
	static char *apCenter = "Atlanta Processing Center";
	static char *sjpCenter = "San Jose Processing Center";
	switch(code){
		case 100:
			return bpCenter;
		case 135:
			if(subCode < 35){
				return dpCenter;
			}
			else {
				return bpCenter;
			}
		case 200:
			return dpCenter;
		case 300:
			return apCenter;
		case 400:
			return sjpCenter;
	}
}

char* staticFormat(const char* name, size_t quantity, size_t weight){
	static char buffer[64];
	sprintf(buffer, "Item: %s  Quantity: %zu  Weight: %zu", name, quantity, weight);
	return buffer;
}

char* blanks(int number){
	char* spaces = (char*)malloc(number + 1);
	int i;
	for(i = 0; i < number; i++){
		spaces[i] = ' ';
	}
	spaces[number] = '\0';
	return spaces;
}

int compare(const char *s1, const char *s2){
	return strcmp(s1, s2);
}

char* stringToLower(const char *string){
	char *tmp = (char*)malloc(strlen(string) + 1);
	char *start = tmp;
	while(*string != 0){
		*tmp++ = tolower(*string++);
	}
	*tmp = 0;
	return start;
}

int compareIgnoreCase(const char *s1, const char *s2){
	char* t1 = stringToLower(s1);
	char* t2 = stringToLower(s2);
	int result = strcmp(t1, t2);
	free(t1);
	free(t2);
	return result;
}

typedef int (*fptrOperation)(const char*, const char*);
void sort(char *array[], int size, fptrOperation operation){
	int swap = 1;
	while(swap){
		swap = 0;
		int i;
		for(i = 0; i < size-1; i++){
			if(operation(array[i], array[i+1]) > 0){
				swap = 1;
				char *tmp = array[i];
				array[i] = array[i+1];
				array[i+1] = tmp;
			} 
		} 
	}
}

void displayNames(char* names[], int size){
	int i;
	for(i = 0; i < size; i++){
		printf("%s ", names[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[]){
	int i;
	for(i = 0; i < argc; i++){
		printf("argv[%d] %s\n", i ,argv[i]);
	}
	printf("\n");
	
	printf("sizeof(char) = %d\n", sizeof(char));
	printf("sizeof('a') = %d\n\n", sizeof('a'));
	
//  Changing string constant is not permitted
//	char *tabHeader = "Sound";
//	*tabHeader = 'L';
//	printf("%s\n", tabHeader);

	char header_1[] = "Media Player";
	printf("%s\n", header_1);
	char header_2[13];
	strcpy(header_2, "Media Player");
	printf("%s\n", header_2);
	char header_3[13];
	header_3[0] = 'M';
	header_3[1] = 'e';
	header_3[2] = 'd';
	header_3[3] = 'i';
	header_3[4] = 'a';
	header_3[5] = ' ';
	header_3[6] = 'P';
	header_3[7] = 'l';
	header_3[8] = 'a';
	header_3[9] = 'y';
	header_3[10] = 'e';
	header_3[11] = 'r';
	header_3[12] = '\0';
	printf("%s\n", header_3);
	char *header_4 = (char*)malloc(strlen("Media Player") + 1);
	strcpy(header_4, "Media Player");
	printf("%s\n", header_4);
	free(header_4);
	char *header_5 = (char*)malloc(13);
	*(header_5 + 0) = 'M';
	*(header_5 + 1) = 'e';
	*(header_5 + 2) = 'd';
	*(header_5 + 3) = 'i';
	*(header_5 + 4) = 'a';
	*(header_5 + 5) = ' ';
	*(header_5 + 6) = 'P';
	*(header_5 + 7) = 'l';
	*(header_5 + 8) = 'a';
	*(header_5 + 9) = 'y';
	*(header_5 + 10) = 'e';
	*(header_5 + 11) = 'r';
	*(header_5 + 12) = '\0';
	printf("%s\n\n", header_5);
	free(header_5);
	
	//int strcmp(const char *s1, const char *s2);
	char command[16];
	printf("Enter a Command: ");
	scanf("%s", command);
	if(strcmp(command, "Quit") == 0){
		printf("The command was Quit\n\n");
	}
	else {
		printf("The command was not Quit\n\n");
	}
	
	//char* strcpy(char *s1, const char *s2);
	char name[32];
	char *names[30];
	size_t count = 0;
	printf("Enter a name:");
	scanf(" %s", name);
	names[count] = (char*)malloc(strlen(name) + 1);
	strcpy(names[count], name);
	count++;
	for(i = 0; i < count; i++){
		free(names[i]);
	}
	
	char *pageHeaders[300];
	pageHeaders[12] = "Amorphous Compounds";
	pageHeaders[13] = pageHeaders[12];
	printf("%p\n", pageHeaders[12]);
	printf("%p\n\n", pageHeaders[13]);
	
	//char* strcat(char *s1, const char *s2);
	char* error = "ERROR: ";
	char* errorMessage = "Not enough memory";
	char* buffer = (char*)malloc(strlen(error) + strlen(errorMessage) + 1);
	strcpy(buffer, error);
	strcat(buffer, errorMessage);
	printf("%s\n", buffer);
	printf("%s\n", error);
	printf("%s\n\n", errorMessage);
	free(buffer);
	
//	Error!	
//	char Error = "ERROR: ";
//	char* ErrorMessage = "Not enough memory";
//	strcat(Error, ErrorMessage);
//	printf("%s\n", error);
//	printf("%s\n\n", errorMessage);

	char* path = "C:";
	char* currentPath = (char*)malloc(strlen(path) + 2);
	currentPath = strcpy(currentPath, path);
	currentPath = strcat(currentPath, "\\");
	printf("%s\n\n", currentPath);
	free(currentPath);
	
	char simpleArray[] = "simple string";
	printf("%zu\n", stringLength(simpleArray));
	printf("%zu\n", stringLength(&simpleArray));
	printf("%zu\n", stringLength(&simpleArray[0]));
	char *simplePtr = (char*)malloc(strlen("simple string")+1);
	strcpy(simplePtr, "simple string");
	printf("%zu\n\n", stringLength(simplePtr));
	free(simplePtr);
	
	char buff[40];
	printf("%s\n", format(buff, sizeof(buff), "Axle", 23, 45));
	char *Buff = NULL;
	printf("%s\n\n", Format(Buff, 0, "Axle", 23, 45));
	free(Buff);
	
	char *part1 = staticFormat("Axle",25,45);
	char *part2 = staticFormat("Piston",55,5);
	printf("%s\n",part1);	//Item: Piston  Quantity: 55  Weight: 5
	printf("%s\n\n",part2);	//Item: Piston  Quantity: 55  Weight: 5
	
	char *tmp = blanks(5);
	printf("[%s]\n\n", tmp);
	free(tmp);
	
	char *Names[] = {"Bob", "Ted", "Carol", "Alice", "alice"};
	sort(Names,5,compare);
	displayNames(Names,5);
	sort(Names,5,compareIgnoreCase);
	displayNames(Names,5);
	
	system("PAUSE");
	return 0;
}

size_t stringLength(const char* string){
	size_t length = 0;
	while(*(string++)){
		length++;
	}
	return length;
}

