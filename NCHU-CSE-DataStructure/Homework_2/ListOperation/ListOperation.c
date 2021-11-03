#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define FreeLinkedList(list) freeLinkedList(&list)
#define INPUT_FILE "test_case_1-1/input_1.txt" 
#define OUTPUT_FILE "test_case_1-1/output_1.txt"
#define TRUE 1
#define EOS '\0'

FILE *fin, *fout;


typedef struct listNode *ListPointer;
typedef struct listNode {
	int data;
	ListPointer link;
} ListNode;

typedef struct linkedList {
	ListPointer Head;
	int numOfList;
} LinkedList;

void linkedListInit(LinkedList*);
void constructLinkedList(LinkedList*, char*);
void instructionProcess(LinkedList*, char*);
void freeLinkedList(LinkedList**);

int main() {
	
	// detect whether OUTPUT_FILE has already existed
	if(!((fout = fopen(OUTPUT_FILE, "r")) == NULL)) {
		fclose(fin);
		printf("File %s has already existed! Overwrite?[Y/n] ", OUTPUT_FILE);
		char Overwrite[10];
		gets(Overwrite);
		int i = 0;
		while(isspace(Overwrite[i])) {
			i++;
		}
		if(!(Overwrite[i] == 'y' || Overwrite[i] == 'Y')) {
			exit(EXIT_SUCCESS);
		}
	}

	
	// input file opened with mode "r"
	if((fin = fopen(INPUT_FILE, "r")) == NULL) {
		printf("Fail to open file %s!", INPUT_FILE);
		exit(EXIT_FAILURE);
	}
	
	// output file opened with mode "w"
	//fout = fopen(OUTPUT_FILE, "w");
	
	LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
	linkedListInit(list);
	
	char charBuff;
	int lenOfFirstLine = 1;	// 1 for '\0'
	while((charBuff = fgetc(fin)) != '\n') {
		lenOfFirstLine++;
	}
	rewind(fin);
	char *firstLine = (char*)malloc(lenOfFirstLine * sizeof(char));
	fscanf(fin, "%[^\n]", firstLine);
	charBuff = fgetc(fin); // buffer '\n'
	constructLinkedList(list, firstLine);
	free(firstLine);
	
	char instructionBuff[50];
	int numOfInstruction;
	fscanf(fin, "%d", &numOfInstruction);
	charBuff = fgetc(fin); // buffer '\n'
	int i;
	for(i = 0; i < numOfInstruction; i++) {
		fscanf(fin, "%[^\n]", instructionBuff);
		charBuff = fgetc(fin); // buffer '\n'
		instructionProcess(list, instructionBuff);
	}
	
	FreeLinkedList(list);
	
	// close files
	fclose(fin);
	//fclose(fout);

	system("PAUSE");
	return 0;
}


void linkedListInit(LinkedList *list) {
	list->Head = NULL;
	list->numOfList = 0;
}

int strToInt(char*);
void add(LinkedList*, char*);
void del(LinkedList*, char*);
void ins();
void mul(LinkedList*, char*, char*);
void rev();
void show(LinkedList*);

enum readListData {
	preData,
	Blank,
	endRead,
};

void constructLinkedList(LinkedList *list, char *listData) {
	enum readListData state;
	int i = 0;
	while(TRUE) {
		state = preData;
		add(list, &listData[i]);
		for(;TRUE;i++) {
			if(state == Blank) {
				break;
			}
			if(listData[i] == EOS) {
				state = endRead;
				break;
			}
			if(isspace(listData[i])) {
				state = Blank;
			}
		}
		if(state == endRead) {
			break;
		}
	}
}

void instructionProcess(LinkedList *list, char *instruction) {
	switch(instruction[0]) {
		case 'a':/* add */
			add(list, &instruction[4]);
			break;
		case 'd':/* del */
			del(list, &instruction[4]);
			break;
		case 'i':/* ins */
			break;
		case 'm':/* mul */
			break;
		case 'r':/* rev */
			break;
		case 's':/* show */
			show(list);
			break;
	}
}

void freeLinkedList(LinkedList **list) {
	ListPointer temp;
	while((*list)->Head) {
		temp = (*list)->Head;
		(*list)->Head = (*list)->Head->link;
		free(temp);
	}
	free(*list);
	*list = NULL;
}


void add(LinkedList *list, char *listData) {
	ListPointer aNode = (ListPointer)malloc(sizeof(ListNode));
	aNode->data = strToInt(listData);
	aNode->link = list->Head;
	list->Head = aNode;
	list->numOfList += 1;
}

void del(LinkedList *list, char *asignedOrder) {
	int reverseOrder_preNode = list->numOfList - strToInt(asignedOrder) - 1;
	ListPointer point = list->Head;
	int i;
	for(i = 0; i < reverseOrder_preNode; i++) {
		point = point->link;
	}
	ListPointer temp;
	if(point == list->Head) {
		temp = list->Head;
		list->Head = list->Head->link;
	}
	else {
		temp = point->link;
		point->link = point->link->link;
	}
	free(temp);
	list->numOfList -= 1;
}

void show(LinkedList *list) {
	ListPointer temp = list->Head;
	int *showBuffer = (int*)malloc(list->numOfList * sizeof(int));
	int i;
	for(i = list->numOfList-1; i >= 0; i--) {
		showBuffer[i] = temp->data;
		temp = temp->link;
	}
	printf("%d", showBuffer[0]);
	for(i = 1; i < list->numOfList; i++) {
		printf(" %d", showBuffer[i]);
	}
	printf("\n");
	free(showBuffer);
}

int strToInt(char *str) {
	int returnInt = 0;	
	int i;
	for(i = 0; TRUE; i++) {
		if(isspace(str[i]) || str[i] == '\0') {
			break;
		}
		returnInt *= 10;
		returnInt += (str[i] - '0');
	}
	return returnInt;
}



