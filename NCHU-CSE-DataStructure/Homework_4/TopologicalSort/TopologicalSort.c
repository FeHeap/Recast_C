#include <stdio.h>
#include <stdlib.h>
#define INPUT_FILE "test_case_1-3/input_1.txt" 
#define TRUE 1

FILE *fin;

typedef struct node {
	int myself;
	int top;
	int full;
	int *pointTo;
} Node;

typedef struct stack {
	int num;
	struct Stack *next;
} Stack;


int main() {
	// input file opened with mode "r"
	if((fin = fopen(INPUT_FILE, "r")) == NULL) {
		printf("Fail to open file %s!", INPUT_FILE);
		exit(EXIT_FAILURE);
	}

	printf("Processing data...\n");
	
	int numOfNode, numOfEdge;
	char charBuff;
	fscanf(fin, "%d", &numOfNode);
	charBuff = fgetc(fin);
	fscanf(fin, "%d", &numOfEdge);
	charBuff = fgetc(fin);
	
	Node *nodes = (Node*)malloc((numOfNode+1) * sizeof(Node));
	int *bePointNode = (int*)calloc((numOfNode+1), sizeof(int)); // to count the times which node be point
	
	int i, j, k;
	/* initialize nodes */
	for(i = 1; i <= numOfNode; i++) {
		nodes[i].myself = i;
		nodes[i].top = 0;
		nodes[i].full = 4;
		nodes[i].pointTo = (int*)malloc(4 * sizeof(int));
	}
	
	/* get all the edges */
	int point, bePoint;
	int *top;
	for(i = 0; i < numOfEdge; i++) {
		fscanf(fin, "%d", &point);
		charBuff = fgetc(fin);
		fscanf(fin, "%d", &bePoint);
		charBuff = fgetc(fin);
		
		top = &nodes[point].top;
		
		if(*top == nodes[point].full) {
			nodes[point].full *= 2;
			int *temp = nodes[point].pointTo;
			nodes[point].pointTo = (int*)malloc(nodes[point].full * sizeof(int));
			for(j = 0; j < *top; j++) {
				nodes[point].pointTo[j] = temp[j];
			}
			free(temp);
		}
		nodes[point].pointTo[(*top)++] = bePoint;
		
		bePointNode[bePoint]++;
	}
	
	// close file
	fclose(fin);
	
	/* store the data waits for output */
	int *heap = (int*)malloc(numOfNode * sizeof(int));
	int heapTop = 1;
	/* push the first into heap */
	for(i = numOfNode; i >= 1; i--) {
		if(bePointNode[i] == 0) {
			heap[heapTop++] = i;
			int heapPoint = heapTop-1;
			while(heapPoint > 1) {
				if(heap[heapPoint] < heap[heapPoint/2]) {
					int temp = heap[heapPoint];
					heap[heapPoint] = heap[heapPoint/2];
					heap[heapPoint/2] = temp;
				}
				heapPoint /= 2;
			}
		}
	}
	
	/* data for output */
	int *output = (int*)malloc(numOfNode * sizeof(int));
	int outputTail = 0;
	
	while(heapTop != 1) {
		
		output[outputTail++] = heap[1]; // get the smallest node in heap to output
		Node *nodeBuff = &nodes[heap[1]]; // , and buffer that one
		
		/* matain heap */
		heap[1] = heap[--heapTop];
		int heapPoint = 1;
		while(TRUE) {
			if(heapPoint*2+1 < heapTop) {
				int mayChange = (heap[heapPoint*2] < heap[heapPoint*2+1])? heapPoint*2 : heapPoint*2+1;
				if(heap[heapPoint] > heap[mayChange]) {
					int temp = heap[heapPoint];
					heap[heapPoint] = heap[mayChange];
					heap[mayChange] = temp;
					heapPoint = mayChange;
				}
				else {
					break;
				}
			}
			else if(heapPoint*2 < heapTop) {
				if(heap[heapPoint] > heap[heapPoint*2]) {
					int temp = heap[heapPoint];
					heap[heapPoint] = heap[heapPoint*2];
					heap[heapPoint*2] = temp;
					heapPoint *= 2;
				}
				else {
					break;
				}
			}
			else {
				break;	
			}
		}
		
		/* process the edge around that one */
		for(i = 0; i < nodeBuff->top; i++) {
			bePointNode[nodeBuff->pointTo[i]]--;
			if(bePointNode[nodeBuff->pointTo[i]] == 0) { // means that all the node point to it have been done
				heap[heapTop++] = nodeBuff->pointTo[i];
				heapPoint = heapTop-1;
				/* mantain heap */
				while(heapPoint > 1) {
					if(heap[heapPoint] < heap[heapPoint/2]) {
						int temp = heap[heapPoint];
						heap[heapPoint] = heap[heapPoint/2];
						heap[heapPoint/2] = temp;
						heapPoint /= 2;
					}
					else {
						break;
					}
				}
			}
		}
	}
	
	/* output */
	if(outputTail == numOfNode) { // there are loops if and else if outputTail < numOfNode
		printf("%d", output[0]);
		for(i = 1; i < numOfNode; i++) {
			printf(" %d", output[i]);
		}
		printf("\n");
	}
	else {
		printf("not exist\n");
	}
	
	// free
	free(heap);
	free(output);
	for(i = 1; i <= numOfNode; i++) {
		free(nodes[i].pointTo);
	}
	free(nodes);
	free(bePointNode); 
	
	system("PAUSE");
	return 0;
}

