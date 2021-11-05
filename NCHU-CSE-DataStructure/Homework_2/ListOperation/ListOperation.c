#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FreeLinkedList(list) freeLinkedList(&list)
#define INPUT_FILE "test_case_1-3/input_1.txt" 
#define OUTPUT_FILE "test_case_1-3/output_1.txt"
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
	fout = fopen(OUTPUT_FILE, "w");
	
	printf("Processing data...\n");
	
	/* create and initialize list */
	LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
	linkedListInit(list);
	
	
	char charBuff;
	// count the number of chars in first line
	int lenOfFirstLine = 1;	// 1 for EOS
	while((charBuff = fgetc(fin)) != '\n') {
		lenOfFirstLine++;
	}
	rewind(fin);
	char *firstLine = (char*)malloc(lenOfFirstLine * sizeof(char));
	fscanf(fin, "%[^\n]", firstLine);	// read first line
	charBuff = fgetc(fin); // buffer '\n'
	/* build initial list with data in the first line */
	constructLinkedList(list, firstLine);
	free(firstLine);
	
	int numOfInstruction;
	fscanf(fin, "%d", &numOfInstruction);	// read the number of instruction
	charBuff = fgetc(fin); // buffer '\n'
	char instructionBuff[50];
	int i;
	for(i = 0; i < numOfInstruction; i++) {
		fscanf(fin, "%[^\n]", instructionBuff);	// read instruction
		charBuff = fgetc(fin); // buffer '\n'
		instructionProcess(list, instructionBuff); // modify the list with the instruction having read just now
	}
	
	/* free list*/
	FreeLinkedList(list);
	
	// close files
	fclose(fin);
	fclose(fout);
	
	printf("Finish processing the data and the instructions and save the outcome into %s...\n", OUTPUT_FILE);

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
void ins(LinkedList*, char*);
void mul(LinkedList*, char*);
void rev(LinkedList*, char*);
void show(LinkedList*);

enum readListData {
	preData,
	Blank,
	endRead,
};

void constructLinkedList(LinkedList *list, char *listData) {
	if(strlen(listData) == 0) { return; }
	enum readListData state;
	int i = 0;
	while(TRUE) {
		state = preData;
		add(list, &listData[i]);
		for(;TRUE;i++) {
			if(state == Blank && !isspace(listData[i])) {
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
	
	int i;
	
	switch(instruction[0]) {
		case 's':/* show */
			show(list);
			break;
		
		default:
			i = 0;
			while(!isspace(instruction[i++]));
			switch(instruction[0]) {
				case 'a':/* add */
					add(list, &instruction[i]);
					break;
				case 'd':/* del */
					del(list, &instruction[i]);
					break;
				case 'i':/* ins */
					ins(list, &instruction[i]);
					break;
				case 'm':/* mul */
					mul(list, &instruction[i]);
					break;
				case 'r':/* rev */
					rev(list, &instruction[i]);
					break;
			}
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

/* show */
void show(LinkedList *list) {
	ListPointer temp = list->Head;
	int *showBuffer = (int*)malloc(list->numOfList * sizeof(int));
	int i;
	for(i = list->numOfList-1; i >= 0; i--) {
		showBuffer[i] = temp->data;
		temp = temp->link;
	}
	for(i = 0; i < list->numOfList; i++) {
		fprintf(fout, "%d ", showBuffer[i]);
	}
	fprintf(fout, "\n");
	free(showBuffer);
}

/* add */
void add(LinkedList *list, char *listData) {
	ListPointer aNode = (ListPointer)malloc(sizeof(ListNode));
	aNode->data = strToInt(listData);
	aNode->link = list->Head;
	list->Head = aNode;
	list->numOfList += 1;
}

/* del */
void del(LinkedList *list, char *asignedOrder) {
	int order = strToInt(asignedOrder);
	if(order > list->numOfList) { return; }
	int reverseOrder_preNode = list->numOfList - order - 1;
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

/* ins */
void ins(LinkedList *list, char *parements) {
	int order = strToInt(parements);
	int i = 0;
	while(!isspace(parements[i++]));
	int data = strToInt(&parements[i]);
	if(order >= list->numOfList) {
		add(list, &parements[i]);
	}
	else {
		int reverseOrder_preNode = list->numOfList - order - 1;
		ListPointer point = list->Head;
		for(i = 0; i < reverseOrder_preNode; i++) {
			point = point->link;
		}
		ListPointer temp = (ListPointer)malloc(sizeof(ListNode));
		temp->data = data;
		temp->link = point->link;
		point->link = temp;
		list->numOfList += 1;
	}
}

/* mul */
void mul(LinkedList *list, char *parements) {
	int order = strToInt(parements);
	if(order > list->numOfList) { return; }
	int reverseOrder_preNode = order - 1;
	int i = 0;
	while(!isspace(parements[i++]));
	int multiple = strToInt(&parements[i]);
	ListPointer temp = list->Head;
	for(i = 0; i < reverseOrder_preNode; i++) {
		temp = temp->link;
	}
	temp->data *= multiple;
}

/* rev */
void rev(LinkedList *list, char *parement) {
	ListPointer point = list->Head;
	ListPointer temp = point;
	
	int block = strToInt(parement);
	int *dataBuff = (int*)malloc(block * sizeof(int));
	int lastPartOfList = list->numOfList % block;
	int i, j;
	if(!(lastPartOfList == 0)) {
		for(i = 0; i < lastPartOfList; i++) {
			dataBuff[i] = temp->data;
			temp = temp->link;
		}
		for(i = lastPartOfList-1; i >= 0; i--) {
			point->data = dataBuff[i];
			point = point->link;
		}
	}
	int numOfBlock = list->numOfList / block;
	for(i = 0; i < numOfBlock; i++) {
		for(j = 0; j < block; j++) {
			dataBuff[j] = temp->data;
			temp = temp->link;
		}
		for(j = block-1; j >= 0; j--) {
			point->data = dataBuff[j];
			point = point->link;
		}
	}
	free(dataBuff);
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



