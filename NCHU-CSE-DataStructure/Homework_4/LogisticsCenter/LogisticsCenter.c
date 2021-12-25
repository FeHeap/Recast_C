#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INPUT_FILE "test_case_2-3/input_2.txt" 
#define TRUE 1

FILE *fin;

typedef struct edge {
	int toWhere;
	int length;
} Edge;

typedef struct node {
	int cost; // sum of cost from all destination
		
	int container;	// container of array neighbour
	int top;		// num of neighbour
	Edge *neighbour;
} Node;
Node *Nodes;

int weightInTruck;
void deliverEvaluate(int, int);

int main() {
	// input file opened with mode "r"
	if((fin = fopen(INPUT_FILE, "r")) == NULL) {
		printf("Fail to open file %s!", INPUT_FILE);
		exit(EXIT_FAILURE);
	}

	printf("Processing data...\n");
	
	int numOfNodes, destinations;
	char charBuff;
	fscanf(fin, "%d", &numOfNodes);
	charBuff = fgetc(fin);
	fscanf(fin, "%d", &destinations);
	charBuff = fgetc(fin);
	
	// malloc Nodes
	Nodes = (Node*)malloc((numOfNodes+1) * sizeof(Node));
	
	// declare i, j
	int i, j;
	
	// initialize Nodes
	for(i = 1; i <= numOfNodes; i++) {
		Nodes[i].cost = 0;
		
		Nodes[i].top = 0;
		Nodes[i].container = 2;
		Nodes[i].neighbour = (Edge*)malloc(2 * sizeof(Edge));
	}
	
	// set edges to their two nodes
	int twoNodes[2], distance;
	for(i = 1; i < numOfNodes; i++) {
		// one Node
		fscanf(fin, "%d", &twoNodes[0]);
		charBuff = fgetc(fin);
		// another Node
		fscanf(fin, "%d", &twoNodes[1]);
		charBuff = fgetc(fin);
		// distance betweeen two Nodes
		fscanf(fin, "%d", &distance);
		charBuff = fgetc(fin);
		
		// process data of two Nodes
		for(j = 0; j < 2; j++) {
			if(Nodes[twoNodes[j]].top == Nodes[twoNodes[j]].container) {
				Nodes[twoNodes[j]].container *= 2;
				Nodes[twoNodes[j]].neighbour = (Edge*)realloc(Nodes[twoNodes[j]].neighbour, Nodes[twoNodes[j]].container * sizeof(Edge));
			}
			// !0 == 1, !1 == 0
			Nodes[twoNodes[j]].neighbour[Nodes[twoNodes[j]].top].toWhere = twoNodes[!j];
			Nodes[twoNodes[j]].neighbour[Nodes[twoNodes[j]].top++].length = distance;
		}
	}
	
	// Deliver Evaluate for every destination to all nodes by DFA 
	int destinationNode, goodsWeight;
	for(i = 0; i < destinations; i++) {
		// destination
		fscanf(fin, "%d", &destinationNode);
		charBuff = fgetc(fin);
		// another Node
		fscanf(fin, "%d", &goodsWeight);
		charBuff = fgetc(fin);
		deliverEvaluate(destinationNode, goodsWeight);
	}
	
	// compare cost of all nodes
	int minCostNode = 1;
	for(i = 1; i <= numOfNodes; i++) {
		if(Nodes[minCostNode].cost > Nodes[i].cost) {
			minCostNode = i;
		}
	}
	
	// print the outcome on the screen
	printf("cost: %d\n", Nodes[minCostNode].cost * 2);
	printf("Logistics Center: %d\n", minCostNode);
	
	// free all dynamic memory
	for(i = 1; i <= numOfNodes; i++) {
		free(Nodes[i].neighbour);
	}
	free(Nodes);

	// close file
	fclose(fin);

	system("PAUSE");
	return 0;
}



void DFA(int from, int here, int toAdd){
	// update cost(+toAdd) 
	Nodes[here].cost += toAdd;
	
	int i;
	int top = Nodes[here].top;
	Edge *Edges = Nodes[here].neighbour;
	// went to other nodes but from
	for(i = 0; i < top; i++) {
		if(Edges[i].toWhere != from) {
			DFA(here, Edges[i].toWhere, toAdd + weightInTruck*Edges[i].length);
		}
	}
}
void deliverEvaluate(int destinationNode, int goodsWeight) {
	// set the weight of goods
	weightInTruck = goodsWeight;
	// DFA from destinationNode
	DFA(0, destinationNode, 0);
}

