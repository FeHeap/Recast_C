#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INPUT_FILE "test_case_3-1/input_3.txt" 

FILE *fin;

// to convert index number by sorting with weight
typedef struct convertUnit {
	int index;
	int weight;
} ConvertUnit;

// node in graph
typedef struct node {
	int area;
	int *neighbour;
} Node;

// color bucket
typedef struct colorBucket {
	int capacity, cost;
} ColorBucket;

void shellSortConvertUnits(ConvertUnit*, int);
void shellSortColorBuckets(ColorBucket*, int);
int countLeastCost(Node*, int, ColorBucket*, int);

int main() {
	// input file opened with mode "r"
	if((fin = fopen(INPUT_FILE, "r")) == NULL) {
		printf("Fail to open file %s!", INPUT_FILE);
		exit(EXIT_FAILURE);
	}

	printf("Processing data...\n");
	
	// read the first line of file
	int numOfNodes, numOfEdge, numOfColor;
	char charBuff;
	fscanf(fin, "%d", &numOfNodes);
	charBuff = fgetc(fin);
	fscanf(fin, "%d", &numOfEdge);
	charBuff = fgetc(fin);
	fscanf(fin, "%d", &numOfColor);
	charBuff = fgetc(fin);
	
	// malloc Nodes
	Node *Nodes = (Node*)malloc((numOfNodes+1) * sizeof(Node));
	
	// ConvertTable is the tool to support to generate indexConvertTable
	ConvertUnit *ConvertTable = (ConvertUnit*)malloc((numOfNodes+1) * sizeof(ConvertUnit));
	
	// declare i, j
	int i, j;
	
	// store the initial Node data into ConvertTable
	for(i = 1; i <= numOfNodes; i++) {
		fscanf(fin, "%d", &ConvertTable[i].weight);
		charBuff = fgetc(fin);
		ConvertTable[i].index = i;
	}
	// initialize ConvertTable[0].weight and sorting ConvertTable
	ConvertTable[0].weight = -1;
	shellSortConvertUnits(ConvertTable, numOfNodes+1);
	
	// initialize Nodes
	for(i = 1; i <= numOfNodes; i++) {
		Nodes[i].area = ConvertTable[i].weight;
		Nodes[i].neighbour = (int*)calloc((numOfNodes+1), sizeof(int));
	}
	
	// set the indexConvertTable to support to convert Nodes' index
	int *indexConvertTable = (int*)malloc((numOfNodes+1) * sizeof(int));
	for(i = 1; i <= numOfNodes; i++) {
		indexConvertTable[ConvertTable[i].index] = i;
	}
	// free ConvertTable after indexConvertTable be generated
	free(ConvertTable);
	
	// set edges to their two nodes
	int twoNodes[2];
	for(i = 0; i < numOfEdge; i++) {
		// one Node
		fscanf(fin, "%d", &twoNodes[0]);
		charBuff = fgetc(fin);
		// another Node
		fscanf(fin, "%d", &twoNodes[1]);
		charBuff = fgetc(fin);
		
		twoNodes[0] = indexConvertTable[twoNodes[0]];
		twoNodes[1] = indexConvertTable[twoNodes[1]];
		
		
		Nodes[twoNodes[0]].neighbour[twoNodes[1]] = 1;
		Nodes[twoNodes[1]].neighbour[twoNodes[0]] = 1;
	}
	// free indexConvertTable after setting Nodes' data is finished
	free(indexConvertTable);
	
	/* // to check the graph(Nodes)
	for(i = 1; i <= numOfNodes; i++) {
		printf("%d:", Nodes[i].area);
		for(j = 1; j < (numOfNodes+1); j++) {
			printf(" %d", Nodes[i].neighbour[j]);
		}
		printf("\n");
	}
	*/
	
	int smallestArea = Nodes[1].area;
	// malloc ColorBuckets
	ColorBucket *ColorBuckets = (ColorBucket*)malloc(numOfColor*sizeof(ColorBucket));
	// numOfColorBeUsed to count the color can be used
	int numOfColorBeUsed = 0;
	char colorBuff[5];
	int capacity, cost;
	for(i = 0; i < numOfColor; i++) {
		// color
		fscanf(fin, "%[^ ]", colorBuff);
		charBuff = fgetc(fin);
		// capacity
		fscanf(fin, "%d", &capacity);
		charBuff = fgetc(fin);
		// cost
		fscanf(fin, "%d", &cost);
		charBuff = fgetc(fin);
		
		// if the capacity not smaller than smallestArea in Nodes, the color is able to be used.
		if(capacity >= smallestArea) {
			ColorBuckets[numOfColorBeUsed].capacity = capacity;
			ColorBuckets[numOfColorBeUsed++].cost = cost;
		}
	}
	
	// close file
	fclose(fin);
	
	// sort the ColorBuckets with cost
	shellSortColorBuckets(ColorBuckets, numOfColorBeUsed);
	/* to check the outcome of sorting
	for(i = 0; i < numOfColorBeUsed; i++) {
		printf("%d %d\n", ColorBuckets[i].cost, ColorBuckets[i].capacity);
	}
	*/
	
	
	int leastCost = countLeastCost(Nodes, numOfNodes, ColorBuckets, numOfColorBeUsed);
	printf("Least Cost: %d\n", leastCost);
	
	// free all dynamic memory
	free(ColorBuckets);
	for(i = 1; i <= numOfNodes; i++) {
		free(Nodes[i].neighbour);
	}
	free(Nodes);


	system("PAUSE");
	return 0;
}

void shellSortConvertUnits(ConvertUnit *ConvertTable, int size) {
	int MarcinCiura[] = {1750, 701, 301, 132, 57, 23, 10, 4, 1};
	int upperBound = 9;
	int gap,point;
	int i, j;
    for(i = 0;i < upperBound;i++){
        gap = MarcinCiura[i];
        for(j = gap;j < size;j++){
            if(ConvertTable[j].weight < ConvertTable[j-gap].weight){
                for(point = j; point >= gap; point -= gap){
                    if(ConvertTable[point].weight < ConvertTable[point-gap].weight){
                        ConvertUnit temp = ConvertTable[point];
                        ConvertTable[point] = ConvertTable[point-gap];
                        ConvertTable[point-gap] = temp;
                    }
                    else{
                    	break;
                    }
                }
            }
        }
    }
}

void shellSortColorBuckets(ColorBucket *ColorBuckets, int size) {
	int MarcinCiura[] = {1750, 701, 301, 132, 57, 23, 10, 4, 1};
	int upperBound = 9;
	int gap,point;
	int i, j;
    for(i = 0;i < upperBound;i++){
        gap = MarcinCiura[i];
        for(j = gap;j < size;j++){
            if(ColorBuckets[j].cost < ColorBuckets[j-gap].cost){
                for(point = j; point >= gap; point -= gap){
                    if(ColorBuckets[point].cost < ColorBuckets[point-gap].cost){
                        ColorBucket temp = ColorBuckets[point];
                        ColorBuckets[point] = ColorBuckets[point-gap];
                        ColorBuckets[point-gap] = temp;
                    }
                    else{
                    	break;
                    }
                }
            }
        }
    }
}

struct countManager {
	int LeastCost;
	Node *Nodes;
	int numOfNode;
	ColorBucket *ColorBuckets;
	int numOfColorBucket;
	
	int *paintingFlags;
} CountManager;
int costBuff = 0;
void costCount(int numOfArea, int colorDraw) {
	printf("%d,  cost:%d\n", colorDraw, costBuff);
	int *stack = (int*)malloc(CountManager.numOfNode * sizeof(int));
	int top = 0;
	int i, j;
	for(i = 1; i <= CountManager.numOfNode; i++) {
		printf("---------------%d, %d\n", CountManager.ColorBuckets[colorDraw].capacity, CountManager.Nodes[i].area);
		if(CountManager.ColorBuckets[colorDraw].capacity >= CountManager.Nodes[i].area) {
			if(CountManager.paintingFlags[i] == 0) {
				int flag = 1;
				for(j = 0; j < top; j++) {
					if(CountManager.Nodes[stack[j]].neighbour[i] == 1) {
						flag = 0;
						break;
					}
				}
				if(flag) {
					printf(":%d:\n", i);
					stack[top++] = i;
					CountManager.ColorBuckets[colorDraw].capacity -= CountManager.Nodes[i].area;
					costBuff += CountManager.Nodes[i].area * CountManager.ColorBuckets[colorDraw].cost;
					printf("cost:%d   ", costBuff);
					CountManager.paintingFlags[i] = 1;
					numOfArea--;
					for(j = 1; j <= CountManager.numOfNode; j++) {
						if(CountManager.paintingFlags[j] == 1) {
							printf("%d ", CountManager.Nodes[j].area);
						}
					}
					printf("\n");
					if(numOfArea == 0) {
						if(costBuff < CountManager.LeastCost) {
							CountManager.LeastCost = costBuff;
						}
						break;
					}
				}
			}
		}
		else {
			
			if(top == 0) {
				break;
			}
			else {
				if(colorDraw + 1 < CountManager.numOfColorBucket) {
					costCount(numOfArea, colorDraw + 1);
					CountManager.ColorBuckets[colorDraw].capacity += CountManager.Nodes[stack[--top]].area;
					costBuff -= CountManager.Nodes[stack[top]].area * CountManager.ColorBuckets[colorDraw].cost;
					CountManager.paintingFlags[stack[top]] = 0;
					i = stack[top];
					numOfArea++;
				} else {
					break;
				}
			}
		}
	}
	
	printf("%d %d %d\n", i, colorDraw + 1, CountManager.numOfColorBucket);
	if(colorDraw + 1 < CountManager.numOfColorBucket && i == CountManager.numOfNode) {
		costCount(numOfArea, colorDraw + 1);
	}
		
	for(j = 0; j < top; j++) {
		CountManager.paintingFlags[stack[j]] = 0;
		CountManager.ColorBuckets[colorDraw].capacity += CountManager.Nodes[stack[j]].area;
		costBuff -= CountManager.Nodes[stack[j]].area * CountManager.ColorBuckets[colorDraw].cost;
	}
	
	
	
	
	free(stack);
}
int countLeastCost(Node *Nodes, int numOfNode, ColorBucket *ColorBuckets, int numOfColorBucket) {
	printf("Nodes:%d\n", numOfNode);
	CountManager.paintingFlags = (int*)calloc(numOfNode+1, sizeof(int));
	CountManager.LeastCost = INT_MAX;
	CountManager.Nodes = Nodes;
	CountManager.numOfNode = numOfNode;
	CountManager.ColorBuckets = ColorBuckets;
	CountManager.numOfColorBucket = numOfColorBucket;
	costCount(numOfNode, 0);
	
	free(CountManager.paintingFlags);
	CountManager.paintingFlags = NULL;
	
	return CountManager.LeastCost;
}
