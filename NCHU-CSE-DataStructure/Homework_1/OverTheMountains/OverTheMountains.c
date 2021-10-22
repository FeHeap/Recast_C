#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_FILE "input_3.txt"
#define OUTPUT_FILE "output_4.txt"
#define byte char
#define TRUE 1
#define FALSE 0

#define safeFree(p) saferFree((void**)&(p))

FILE *fin, *fout;

typedef struct mountain {
	short cost[8];
} Mountain;


typedef struct map {
	short destinationNum;
	short edge;
	short startAddress[2];
	short destination[5][2];
	short routeCost[6][6];	// start: 0, destination: 1, 2, 3, 4, 5
	Mountain **terrainMap;
} Map;


void saferFree(void**);
short** createInitailMap(short);
Map* createMap(short, short, short*, short(*)[2], short**);

void setMapRouteCost(Map*);
typedef struct queueUnit {
	short weight;
	short YX[2];
} QueueUnit;
void swapQueueUnit(QueueUnit*, QueueUnit*);

typedef struct bfsMapUnit {
	byte isDestination;
	byte havePassed;
} BFSMapUnit;

// compass to detect 8 directions around
short compass[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1},};
short directY, directX;
short compassPoint;

/* Main Function */
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

	byte mapNum;	// the number of maps
	char charBuff;			// to buff blank or '\n'
	fscanf(fin, "%hhd", &mapNum);	// get the number of maps
	charBuff = fgetc(fin);	// buff '\n'
	
	
	// variable to count the times
	short i, j, k;
	short destinationNum, edge;
	short startAddress[2];
	short destination[5][2];

	// which map do the process read
	for(i = 0; i < mapNum; i++) {	// map

		// get the number of the destinations
		fscanf(fin, "%hd", &destinationNum);
		charBuff = fgetc(fin);	// buff blank

		//  get the edge
		fscanf(fin, "%hd", &edge);
		charBuff = fgetc(fin);	// buff '\n'
		
		// get the X
		fscanf(fin, "%hd", &startAddress[1]);
		charBuff = fgetc(fin);	// buff blank
		// get the Y
		fscanf(fin, "%hd", &startAddress[0]);
		charBuff = fgetc(fin);	// buff '\n'
		
		for(j = 0; j < destinationNum; j++) {
			// get the X
			fscanf(fin, "%hd", &destination[j][1]);
			charBuff = fgetc(fin);	// buff blank
			//  get the Y
			fscanf(fin, "%hd", &destination[j][0]);
			charBuff = fgetc(fin);	// buff '\n'
		}
		
		// create a initial map
		short **initialMapData = createInitailMap(edge);
		
		// read the data of the map in INPUT_FILE and store it into variable map in the process
		for(j = 0; j < edge; j++) {		// Y
			for(k = 0; k < edge; k++) {	// X
				fscanf(fin, "%hd", (*(initialMapData+j)+k));	// whether the cell servive
				charBuff = fgetc(fin);	// buff blank or '\n'
			}		
		}
		
		
		Map *aMap = createMap(destinationNum, edge, startAddress, destination, initialMapData);
		
		for(j = 0; j < edge; j++) {		// Y
			for(k = 0; k < edge; k++) {	// X
				printf("%hd ", *(*(initialMapData+j)+k));	// whether the cell servive
			}
			printf("\n");
		}
		
		// free the initial map
		for(j = 0; j < edge; j++) {
			safeFree(*(initialMapData+j));
		}
		safeFree(initialMapData);
		
//		for(j = 0; j < edge; j++) {		// Y
//			for(k = 0; k < edge; k++) {	// X
//				int l;
//				for(l = 0; l < 8; l++)
//					printf("%hd ", (*(aMap->terrainMap+j)+k)->cost[l]);	// whether the cell servive
//				printf("\n");
//			}
//		}

		setMapRouteCost(aMap);
		
		printf("----------\n");
		
		for(j = 0; j < edge; j++) {
			safeFree(*(aMap->terrainMap+j));
		}
		safeFree(aMap->terrainMap);
		safeFree(aMap);
	}
	
	printf("Finding the path with the least consumption in each map...\n");

	
	// close files
	fclose(fin);
	fclose(fout);
	
	printf("Successfully figure out the least consumption to reach all the destinations of each map and store the outcome into %s\n", OUTPUT_FILE);
	
	system("PAUSE");
	return 0;
} 

void saferFree(void **pp){
	// printf("SAFER FREE\n");
	if(pp != NULL && *pp != NULL){
		free(*pp);
		*pp = NULL;
	}
}

short** createInitailMap(short edge) {
	
	short i;
	
	short **initialMapData = (short**)malloc(edge * sizeof(short*));
	
	for(i = 0; i < edge; i++) {
		*(initialMapData+i) = (short*)malloc(edge * sizeof(short));
	}
	
	return initialMapData;
}

Map* createMap(short destinationNum, short edge, short *startAddress, short (*destination)[2], short** initialMapData) {
	
	Map *map = (Map*)malloc(sizeof(Map));
	map->destinationNum = destinationNum;
	map->edge = edge;
	map->startAddress[0] = startAddress[0];
	map->startAddress[1] = startAddress[1];
	
	short i, j;
	for(i = 0; i < destinationNum; i++) {
		map->destination[i][0] = destination[i][0];
		map->destination[i][1] = destination[i][1];
	}
	
	for(i = 0; i <= destinationNum; i++) {
		for(j = 0; j <= destinationNum; j++) {
			map->routeCost[i][j] = 0;
		}
	}
	
	Mountain** terrainMap = (Mountain**)malloc(edge * sizeof(Mountain*));
	for(i = 0; i < edge; i++) {
		*(terrainMap+i) = (Mountain*)malloc(edge * sizeof(Mountain));
	}

	for(i = 0; i < edge; i++) {
		for(j = 0; j < edge; j++) {
			//printf("Y: %d X: %d    ", i, j);
			for(compassPoint = 0; compassPoint < 8; compassPoint++) {
				directY = compass[compassPoint][0] + i;	// directY buffer the Y coordinate of the position detected
				directX = compass[compassPoint][1] + j;	// directX buffer the X coordinate of the position detected
				if(directY < 0 || directX < 0 || directY == edge || directX == edge) { // if out of boundary, continue
					(*(terrainMap+i)+j)->cost[compassPoint] = -1;
				}
				else {
					(*(terrainMap+i)+j)->cost[compassPoint] = abs(initialMapData[i][j] - initialMapData[directY][directX]);
				}
				//printf("%d ", (*(terrainMap+i)+j)->cost[compassPoint]);
			}
			//printf("\n");
		}	
	}
	
	map->terrainMap = terrainMap;
	
	return map;
}

void setMapRouteCost(Map *map) {
	QueueUnit *priorityQueue = (QueueUnit*)malloc((16 * map->edge + 1) * sizeof(QueueUnit));
	priorityQueue[0].weight = -1;
	priorityQueue[0].YX[0] = -1;
	priorityQueue[0].YX[1] = -1;
	short top;
	
	short specialPoint[6][2];
	specialPoint[0][0] = map->startAddress[0];
	specialPoint[0][1] = map->startAddress[1];
	short i, j, k;
	for(i = 1; i <= map->destinationNum; i++) {
		specialPoint[i][0] = map->destination[i-1][0];
		specialPoint[i][1] = map->destination[i-1][1];
	}
	
	BFSMapUnit **BFSMap = (BFSMapUnit**)malloc(map->edge * sizeof(BFSMapUnit*));
	for(i = 0; i < map->edge; i++) {
		*(BFSMap+i) = (BFSMapUnit*)malloc(map->edge * sizeof(BFSMapUnit));
	}
	for(i = 0; i < map->edge; i++) {
		for(j = 0; j < map->edge; j++) {
			BFSMap[i][j].isDestination = FALSE;
		}
	}
	for(i = 1; i <= map->destinationNum; i++) {
		BFSMap[specialPoint[i][0]][specialPoint[i][1]].isDestination = TRUE;
	}
	
	for(i = 0; i < map->destinationNum; i++) {
		for(j = 0; j < map->edge; j++) {
			for(k = 0; k < map->edge; k++) {
				BFSMap[j][k].havePassed = FALSE;
			}
		}
		BFSMap[specialPoint[i][0]][specialPoint[i][1]].isDestination = FALSE;
		top = 1;
		QueueUnit *noteLeft, *noteRight;
		
		priorityQueue[top].weight = 0;
		priorityQueue[top].YX[0] = specialPoint[i][0];
		priorityQueue[top++].YX[1] = specialPoint[i][1];
		short specialPointNum = map->destinationNum-i;
		printf("===i: %hd===\n", i);
		while(top != 1)	{
			QueueUnit now = { priorityQueue[1].weight, {priorityQueue[1].YX[0], priorityQueue[1].YX[1]}};
			printf("%hd %hd %hd top: %hd\n", now.weight, now.YX[0], now.YX[1], top);
			byte haveAlreadyPassed = FALSE;
			if(BFSMap[now.YX[0]][now.YX[1]].havePassed == FALSE) {
				BFSMap[now.YX[0]][now.YX[1]].havePassed = TRUE;
				if(BFSMap[now.YX[0]][now.YX[1]].isDestination == TRUE) {
					specialPointNum--;
					for(j = i+1; j <= map->destinationNum; j++) {
						if(specialPoint[j][0] == now.YX[0] && specialPoint[j][1] == now.YX[1])
							break;
					}
					map->routeCost[i][j] = now.weight;
					printf("^-----HERE\n");
				}
				if(specialPointNum == 0) {
					break;
				}
			}
			else {
				haveAlreadyPassed = TRUE;
			}
			
			priorityQueue[1] = priorityQueue[--top];
			short sinkDown = 1;
			while(TRUE) {
				if(2*sinkDown+1 < top) {
					noteLeft = &priorityQueue[2*sinkDown];
					noteRight = &priorityQueue[2*sinkDown+1];
					if(priorityQueue[sinkDown].weight > noteLeft->weight || priorityQueue[sinkDown].weight > noteRight->weight) {
						
						if(noteLeft->weight <= noteRight->weight) {
							swapQueueUnit((priorityQueue + sinkDown), noteLeft);
							sinkDown = 2*sinkDown;
						}
						else {
							swapQueueUnit((priorityQueue + sinkDown), noteRight);
							sinkDown = 2*sinkDown + 1;
						}
						
					}
					else {
						break;
					}
				}
				else if(2*sinkDown < top) {
					noteLeft = &priorityQueue[2*sinkDown];
					swapQueueUnit((priorityQueue + sinkDown), noteLeft);
					sinkDown = 2*sinkDown;
				}
				else {
					break;
				}
			}
			
			if(haveAlreadyPassed) {
				continue;
			}
			
			for(compassPoint = 0; compassPoint < 8; compassPoint++) {
				if(map->terrainMap[now.YX[0]][now.YX[1]].cost[compassPoint] == -1) { // if out of boundary, continue
					//pass
				}
				else {
					directY = compass[compassPoint][0] + now.YX[0];	// directY buffer the Y coordinate of the position detected
					directX = compass[compassPoint][1] + now.YX[1];	// directX buffer the X coordinate of the position detected
					if(!BFSMap[directY][directX].havePassed){
						short floatUp = top;
						priorityQueue[top].weight = now.weight + map->terrainMap[now.YX[0]][now.YX[1]].cost[compassPoint];
						priorityQueue[top].YX[0] = directY;
						priorityQueue[top++].YX[1] = directX;
						while(floatUp != 1) {
							if(priorityQueue[floatUp].weight < priorityQueue[floatUp/2].weight) {
								swapQueueUnit((priorityQueue + floatUp), (priorityQueue + floatUp/2));
								floatUp /= 2;
							}
							else {
								break;
							}
						}
					}
				}
			}
			
		}
	}
	
	printf("=======\n");
	for(i = 0; i <= map->destinationNum; i++) {
		for(j = 0; j <= map->destinationNum; j++) {
			printf("%hd ", map->routeCost[i][j]);
		}
		printf("\n");
	}
	printf("=======\n");
	
//	printf("RR\n");
//	for(i = 0; i < 4 * map->edge + 1; i++) {
//		printf("%d weight: %hd, (%hd, %hd)\n", i, priorityQueue->weight, priorityQueue->YX[0], priorityQueue->YX[1]);
//	}
	safeFree(priorityQueue);
//	printf("RR\n");
	for(i = 0; i < map->edge; i++) {
		safeFree(*(BFSMap+i));
	}
	safeFree(BFSMap);
}

void swapQueueUnit(QueueUnit *A, QueueUnit *B) {
	QueueUnit temp = *A;
	*A = *B;
	*B = temp;
}

