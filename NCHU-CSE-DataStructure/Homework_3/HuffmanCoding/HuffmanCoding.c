#include <stdio.h>
#include <stdlib.h>
#define INPUT_FILE "test_case_2-2/input_2.txt" 
#define TRUE 1
#define NumOfCharType 128
FILE *fin;

typedef struct charType {
	char whichChar;
	struct charType *next;
} CharType;


typedef struct heapUnit {
	int sumOfNum;
	CharType *types;
} HeapUnit;


int main() {
	
	// input file opened with mode "r"
	if((fin = fopen(INPUT_FILE, "r")) == NULL) {
		printf("Fail to open file %s!", INPUT_FILE);
		exit(EXIT_FAILURE);
	}

	printf("Processing data...\n");
	
	int numOfLines;
	char charBuff;
	while(TRUE) {
		fscanf(fin, "%d", &numOfLines);
		charBuff = fgetc(fin);
		if(numOfLines == 0) {
			break;
		}
		int i;
		int *charOccurNum = (int*)calloc(NumOfCharType, sizeof(int));
//		printf("\n***\n");
//		for(i = 0; i < NumOfCharType; i++) {
//			printf("%d ", charOccurNum[i]);
//		}
//		printf("\n***\n");
		for(i = 0; i < numOfLines; i++) {
			while((charBuff = fgetc(fin)) != '\n') {
				charOccurNum[charBuff] += 1;
			}
		}
		
		int numOfOccurCharType = 1;	// the index 0 of heap
		for(i = 0; i < NumOfCharType; i++) {
			if(charOccurNum[i] > 0) {
				numOfOccurCharType++;
			}
		}
		
		HeapUnit *charOccurHeap = (HeapUnit*)malloc(numOfOccurCharType * sizeof(HeapUnit));
		int top = 1;
		for(i = 0; i < NumOfCharType; i++) {
			if(charOccurNum[i] > 0) {
				charOccurHeap[top].sumOfNum = charOccurNum[i];
				charOccurHeap[top].types = (CharType*)malloc(sizeof(CharType));
				charOccurHeap[top].types->whichChar = (char)i;
				charOccurHeap[top++].types->next = NULL;
				
				int point = top - 1;
				while(point > 1) {
					if(charOccurHeap[point].sumOfNum < charOccurHeap[point/2].sumOfNum) {
						HeapUnit temp = charOccurHeap[point];
						charOccurHeap[point] = charOccurHeap[point/2];
						charOccurHeap[point/2] = temp;
						point /= 2;
					}
					else {
						break;
					}
				}
			}
		}
		
		int *bitNumOfOccurChar = (int*)calloc(0, NumOfCharType * sizeof(int));
		
		while(top != 2) {
			HeapUnit minSet_1 = charOccurHeap[1];
			charOccurHeap[1] = charOccurHeap[--top];
			int point = 1;
			while(TRUE) {
				if(point * 2 + 1 < top) {
					if(charOccurHeap[point].sumOfNum > charOccurHeap[point * 2].sumOfNum || charOccurHeap[point].sumOfNum > charOccurHeap[point * 2 + 1].sumOfNum) {
						HeapUnit *toChange = (charOccurHeap[point * 2].sumOfNum < charOccurHeap[point * 2 + 1].sumOfNum)? (charOccurHeap + point * 2) : (charOccurHeap + point * 2 + 1);
						HeapUnit temp = *toChange;
						*toChange = charOccurHeap[point];
						charOccurHeap[point] = temp;
						point = (int)(toChange - charOccurHeap);
					}
					else {
						break;
					}
				}
				else if(point * 2 < top) {
					if(charOccurHeap[point].sumOfNum > charOccurHeap[point * 2].sumOfNum) {
						HeapUnit temp = charOccurHeap[point * 2];
						charOccurHeap[point * 2] = charOccurHeap[point];
						charOccurHeap[point] = temp;
						point *= 2;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			
			HeapUnit minSet_2 = charOccurHeap[1];
			charOccurHeap[1] = charOccurHeap[--top];
			point = 1;
			while(TRUE) {
				if(point * 2 + 1 < top) {
					if(charOccurHeap[point].sumOfNum > charOccurHeap[point * 2].sumOfNum || charOccurHeap[point].sumOfNum > charOccurHeap[point * 2 + 1].sumOfNum) {
						HeapUnit *toChange = (charOccurHeap[point * 2].sumOfNum < charOccurHeap[point * 2 + 1].sumOfNum)? (charOccurHeap + point * 2) : (charOccurHeap + point * 2 + 1);
						HeapUnit temp = *toChange;
						*toChange = charOccurHeap[point];
						charOccurHeap[point] = temp;
						point = (int)(toChange - charOccurHeap);
					}
					else {
						break;
					}
				}
				else if(point * 2 < top) {
					if(charOccurHeap[point].sumOfNum > charOccurHeap[point * 2].sumOfNum) {
						HeapUnit temp = charOccurHeap[point * 2];
						charOccurHeap[point * 2] = charOccurHeap[point];
						charOccurHeap[point] = temp;
						point *= 2;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			
//			for(i = 1; i < top; i++) {
//				printf("%d ", charOccurHeap[i].sumOfNum);
//			}
//			printf("\n");
			
			minSet_1.sumOfNum += minSet_2.sumOfNum;
			CharType *pointCharType = minSet_1.types;
			while(pointCharType->next != NULL) {
				bitNumOfOccurChar[pointCharType->whichChar]++;
				pointCharType = pointCharType->next;
			}
			bitNumOfOccurChar[pointCharType->whichChar]++;
			pointCharType->next = minSet_2.types;
			pointCharType = pointCharType->next;
			while(pointCharType != NULL) {
				bitNumOfOccurChar[pointCharType->whichChar]++;
				pointCharType = pointCharType->next;
			}
			
			charOccurHeap[top++] = minSet_1;
			point = top - 1;
			while(point > 1) {
				if(charOccurHeap[point].sumOfNum < charOccurHeap[point/2].sumOfNum) {
					HeapUnit temp = charOccurHeap[point];
					charOccurHeap[point] = charOccurHeap[point/2];
					charOccurHeap[point/2] = temp;
					point /= 2;
				}
				else {
					break;
				}
			}
			
		}
		
		free(charOccurNum);
		CharType *pointCharType = charOccurHeap[1].types;
		while(pointCharType != NULL) {
			CharType *temp = pointCharType;
			pointCharType = pointCharType->next;
			free(temp);
		}
		free(charOccurHeap);
		free(bitNumOfOccurChar);
	}
	
	system("PAUSE");
	return 0;
}

