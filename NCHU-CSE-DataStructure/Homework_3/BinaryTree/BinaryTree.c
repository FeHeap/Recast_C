#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define INPUT_FILE "test_case_1-4/input_1.txt" 
#define OUTPUT_FILE "test_case_1-4/output_1.txt"
#define TRUE 1



FILE *fin, *fout;

typedef struct node {
	int value;
	struct node *leftNode;
	struct node *rightNode;
} Node;

void instructionProcess(Node**, char*);
void insertNodei(Node**, int);
void freeATree(Node**);

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
	
	int times;
	for(times = 1; TRUE; times++) {
		int initialNumOfNodes, numOfInst;
		char charBuff;
		fscanf(fin, "%d", &initialNumOfNodes);
		charBuff = fgetc(fin);
		fscanf(fin, "%d", &numOfInst);
		charBuff = fgetc(fin);
		
		if(initialNumOfNodes == 0 && numOfInst == 0) {
			break;
		}
		if(times != 1) {
			fprintf(fout, "\n");
		}
		fprintf(fout, "# %d", times);
		
		Node *root = NULL;
		
		int i, valueBuff;
		for(i = 0; i < initialNumOfNodes; i++) {
			fscanf(fin, "%d", &valueBuff);
			charBuff = fgetc(fin);
			insertNodei(&root, valueBuff);
		}
		if(initialNumOfNodes == 0) {
			charBuff = fgetc(fin);
		}
		
		char instBuff[20];
		for(i = 0; i < numOfInst; i++) {
			fscanf(fin, "%[^\n]", instBuff);
			charBuff = fgetc(fin);
			instructionProcess(&root, instBuff);
		}
		
		freeATree(&root);
	}
	
	// close files
	fclose(fin);
	fclose(fout);
	
	printf("Finish, and have store the outcome into %s.\n", OUTPUT_FILE);
	
	system("PAUSE");
	return 0;
}

int strToInt(char*);
void insertNode(Node**, char*);
void deleteNode(Node**, char*);
void queryNode(Node**, char*);
void printMaxSum(Node**, char*);

void instructionProcess(Node **rootPoint, char *instruction) {
	
	int  i = 0;
	while(!isspace(instruction[i++]));
	switch(instruction[0]) {
		case 'I':/* add */
			//printf("I\n");
			insertNode(rootPoint, &instruction[i]);
			break;
		case 'D':/* del */
			//printf("D\n");
			deleteNode(rootPoint, &instruction[i]);
			break;
		case 'Q':/* ins */
			//printf("Q\n");
			queryNode(rootPoint, &instruction[i]);
			break;
		case 'P':/* mul */
			//printf("P\n");
			printMaxSum(rootPoint, &instruction[i]);
			break;
		default: // pass
			break;
	}
	
}

void insertNodei(Node **rootPoint, int value) {
	
	Node *NodeBuff = (Node*)malloc(sizeof(Node));
	NodeBuff->value = value;
	NodeBuff->leftNode = NULL;
	NodeBuff->rightNode = NULL;
	
	if(*rootPoint == NULL) {
		*rootPoint = NodeBuff;
	}
	else {
		Node *point = *rootPoint;
		while(TRUE) {
			if(value < point->value) {
				if(point->leftNode) {
					point = point->leftNode;
				}
				else {
					point->leftNode = NodeBuff;
					break;
				}
			}
			else if(value > point->value) {
				if(point->rightNode) {
					point = point->rightNode;
				}
				else {
					point->rightNode = NodeBuff;
					break;
				}
			}
			else {
				free(NodeBuff);
				break;
			}
		}
	}
}

void insertNode(Node **rootPoint, char *data) {
	int value = strToInt(data);
	insertNodei(rootPoint, value);
}

void deleteNode(Node **rootPoint, char *data) {

	int value = strToInt(data);
	
	int matchFlag = 0;
	Node *parent = NULL;
	Node *point = *rootPoint;
	while(TRUE) {
		if(value < point->value) {
			if(point->leftNode) {
				parent = point;
			 	point = point->leftNode;
			}
			else {
			 	break;
			}
		}
		else if(value > point->value) {
			if(point->rightNode) {
				parent = point;
			 	point = point->rightNode;
			}
			else {
			 	break;
			}
		}
		else {
			matchFlag = 1;
			break;
		}
	}
	//printf("%d\n", point->value);
	if(matchFlag == 1) {
		if(point->leftNode) {
			Node *tempPoint = point->leftNode;
			if(tempPoint->rightNode) {
				while(tempPoint->rightNode->rightNode) {
					tempPoint = tempPoint->rightNode;
				}
				
				point->value = tempPoint->rightNode->value;
				Node *subTempPoint = tempPoint->rightNode;
				tempPoint->rightNode = tempPoint->rightNode->leftNode;
				free(subTempPoint);
			}
			else {
				point->value = tempPoint->value;
				point->leftNode = tempPoint->leftNode;
				free(tempPoint);
			}
		}
		else if(point->rightNode) {
			Node *tempPoint = point->rightNode;
			if(tempPoint->leftNode) {
				while(tempPoint->leftNode->leftNode) {
					tempPoint = tempPoint->leftNode;
				}
				
				point->value = tempPoint->leftNode->value;
				Node *subTempPoint = tempPoint->leftNode;
				tempPoint->leftNode = tempPoint->leftNode->rightNode;
				free(subTempPoint);
			}
			else {
				point->value = tempPoint->value;
				point->rightNode = tempPoint->rightNode;
				free(tempPoint);
			}
		}
		else {
			if(parent) {
				if(parent->leftNode == point) {
					parent->leftNode = NULL;
					free(point);
				}
				else {	// parent->rightNode == point
					parent->rightNode = NULL;
					free(point);
				}
			}
			else {
				free(*rootPoint);
				*rootPoint = NULL;
			}
		}
	}
}

void queryNode(Node **rootPoint, char *data) {
	int value = strToInt(data);
	int stage = 0;
	Node *point = *rootPoint;
	while(TRUE) {
		stage++;
		if(value < point->value && point->leftNode) {
			point = point->leftNode;
		}
		else if(value > point->value && point->rightNode) {
			point = point->rightNode;
		}
		else {
			if(point->value != value) {
				stage = 0;	
			}
			break;
		}
	}
	if(stage > 0) {
		fprintf(fout, "\n%d", stage);
	}
}

void printMaxSum(Node **rootPoint, char *data) {
	int  i = 0;
	while(!isspace(data[i++]));
	int v1 = strToInt(data), v2 = strToInt(&data[i]);
	Node *commonRoot = *rootPoint;
	int validFlag = 1;
	while(TRUE) {
		if(commonRoot->value < v1 && commonRoot->value < v2) {
			if(commonRoot->rightNode) {
				commonRoot= commonRoot->rightNode;
			}
			else {
				validFlag = 0;
				break;
			}
		}
		else if(commonRoot->value > v1 && commonRoot->value > v2) {
			if(commonRoot->leftNode) {
				commonRoot= commonRoot->leftNode;
			}
			else {
				validFlag = 0;
				break;
			}
		}
		else {
			break;
		}
	}	
	if(validFlag == 0) {
		return;
	}
	
	int v1PathSum = 0, v2PathSum = 0;
	Node *point = commonRoot;
	while(TRUE) {
		if(point->value > 0) {
			v1PathSum += point->value;
		}
		if(point->value > v1) {
			if(point->leftNode) {
				point = point->leftNode;
			}
			else {
				validFlag = 0;
				break;
			}
		}
		else if(point->value < v1) {
			if(point->rightNode) {
				point = point->rightNode;
			}
			else {
				validFlag = 0;
				break;
			}
		}
		else {
			break;
		}
	}
	if(validFlag == 0) {
		return;
	}
	
	point = commonRoot;
	while(TRUE) {
		if(point->value > 0) {
			v2PathSum += point->value;
		}
		if(point->value > v2) {
			if(point->leftNode) {
				point = point->leftNode;
			}
			else {
				validFlag = 0;
				break;
			}
		}
		else if(point->value < v2) {
			if(point->rightNode) {
				point = point->rightNode;
			}
			else {
				validFlag = 0;
				break;
			}
		}
		else {
			break;
		}
	}
	if(validFlag == 0) {
		return;
	}
	
	int maxSum = v1PathSum + v2PathSum;
	if(commonRoot->value > 0) {
		maxSum -= commonRoot->value;
	}
	
	fprintf(fout, "\n%d", maxSum);
}


int strToInt(char *str) {
	int returnInt = 0;
	int signedFlag = 0;
	if(str[0] == '-') {
		signedFlag = 1;
		str = &str[1];
	}	
	int i;
	for(i = 0; TRUE; i++) {
		if(isspace(str[i]) || str[i] == '\0') {
			break;
		}
		returnInt *= 10;
		returnInt += (str[i] - '0');
	}
	return (signedFlag == 0)? returnInt : (-1)*returnInt;
}

void freeATree(Node **root) {
	
	if(*root == NULL) {
		return;
	}
	
	int size = 50;
	Node **NodeStack = (Node**)malloc(size * sizeof(Node**));
	int i;
	for(i = 0; i < size; i++) {
		NodeStack[i] = NULL;
	}
	
	int top = 0;
	NodeStack[0] = *root;
	do {
		Node *point = NodeStack[top];
		
		if(point->leftNode) {
			if(point->leftNode->leftNode == NULL && point->leftNode->rightNode == NULL) {
				free(point->leftNode);
				point->leftNode = NULL;
			}
			else {
				if(++top == size) {
					size *= 2;
					Node **tempStack = (Node**)malloc(size * sizeof(Node**));
					for(i = 0; i < top; i++) {
						tempStack[i] = NodeStack[i];
					}
					Node **subTempStack = NodeStack;
					NodeStack = tempStack;
					free(subTempStack);
				}
				NodeStack[top] = point->leftNode;
				continue;
			}
		}
		
		if(point->rightNode) {
			if(point->rightNode->leftNode == NULL && point->rightNode->rightNode == NULL) {
				free(point->rightNode);
				point->rightNode = NULL;
			}
			else {
				if(++top == size) {
					size *= 2;
					Node **tempStack = (Node**)malloc(size * sizeof(Node**));
					for(i = 0; i < top; i++) {
						tempStack[i] = NodeStack[i];
					}
					Node **subTempStack = NodeStack;
					NodeStack = tempStack;
					free(subTempStack);
				}
				NodeStack[top] = point->rightNode;
				continue;
			}
		}	
		top--;	
	}while(top != -1);
	

	free(NodeStack);
	free(*root);	
	*root = NULL;
}
