#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_FILE "input_3.txt"
#define OUTPUT_FILE "output_3.txt"
#define byte char
#define TRUE 1
#define FALSE 0
#define SpecialPointNum 6

#define safeFree(p) saferFree((void**)&(p))
#define  CountLeastCost(mapX) countLeastCost(mapX, 0, 0)

#define RAII_VARIABLE(vartype, varname, initval, dtor) \
	void _dtor_ ## varname (vartype * v){ dtor(*v); } \
	vartype varname __attribute__((cleanup(_dtor_ ## varname))) = (initval)

FILE *fin, *fout;

typedef struct mountain {
	short cost[8];	// the cost of the current position to 8 direction
} Mountain;

typedef struct map {
	short leastCost;
	short destinationNum;
	short edge;
	short startAddress[2];
	short destination[SpecialPointNum-1][2];
	short routeCost[SpecialPointNum][SpecialPointNum];	// start: 0, destination: 1, 2, 3, 4, 5, ...
	Mountain **terrainMap;
} Map;

void saferFree(void**);
short** createInitailMap(short);
Map* createMap(short, short, short*, short(*)[2], short**);
void setMapRouteCost(Map*);
void countLeastCost(Map*, short, short);

/* Main Function */
int main() {
	
	// detect whether OUTPUT_FILE has already existed
	if(!((fout = fopen(OUTPUT_FILE, "r")) == NULL)) {
		fclose(fin);
		printf("File %s has already existed! Overwrite?[Y/n] ", OUTPUT_FILE);
		char Overwrite[20];
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

	short mapNum;	// the number of maps
	char charBuff;			// to buff blank or '\n'
	fscanf(fin, "%hd", &mapNum);	// get the number of maps
	charBuff = fgetc(fin);	// buff '\n'
	
	
	printf("Finding the path with the least consumption in each map...\n");
	
	// variable to count the times
	short i, j, k;
	short destinationNum, edge;
	short startAddress[2];
	short destination[SpecialPointNum-1][2];

	// which map do the process read
	for(i = 0; i < mapNum; i++) {	// map

		// get the number of the destinations
		fscanf(fin, "%hd", &destinationNum);
		charBuff = fgetc(fin);	// buff blank

		//  get the edge
		fscanf(fin, "%hd", &edge);
		charBuff = fgetc(fin);	// buff '\n'
		
		// get the X 
		fscanf(fin, "%hd", &startAddress[0]);
		charBuff = fgetc(fin);	// buff blank
		// get the Y 
		fscanf(fin, "%hd", &startAddress[1]);
		charBuff = fgetc(fin);	// buff '\n'
		
		for(j = 0; j < destinationNum; j++) {
			// get the X 
			fscanf(fin, "%hd", &destination[j][0]);
			charBuff = fgetc(fin);	// buff blank
			// get the Y 
			fscanf(fin, "%hd", &destination[j][1]);
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
		
		/* Create aMap */
		Map *aMap = createMap(destinationNum, edge, startAddress, destination, initialMapData);
		
		// free the initial map
		for(j = 0; j < edge; j++) {
			safeFree(*(initialMapData+j));
		}
		safeFree(initialMapData);
		
		/* set aMap RouteCost */ 
		setMapRouteCost(aMap);

		
		/* count aMap LeastCost */ 
		CountLeastCost(aMap);
		
		/* store the outcome(LeastCost) into OUTPUT_FILE */
		fprintf(fout, "#%hd\n", i+1);
		fprintf(fout, "cost:%hd", aMap->leastCost);
		if(i != mapNum-1) {
			fprintf(fout, "\n");
		}
		
		// free lists in terrainMap in aMap
		for(j = 0; j < edge; j++) {
			safeFree(*(aMap->terrainMap+j));
		}
		// free terrainMap in aMap
		safeFree(aMap->terrainMap);
		// free aMap
		safeFree(aMap);
	}

	
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


// compass to detect 8 directions around
short compass[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1},};
short directY, directX;
short compassPoint;

Map* createMap(short destinationNum, short edge, short *startAddress, short (*destination)[2], short** initialMapData) {
	
	Map *map = (Map*)malloc(sizeof(Map));
	
	// initialize all data 
	map->leastCost = SHRT_MAX;
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

	// create terrainMap
	Mountain** terrainMap = (Mountain**)malloc(edge * sizeof(Mountain*));
	for(i = 0; i < edge; i++) {
		*(terrainMap+i) = (Mountain*)malloc(edge * sizeof(Mountain));
	}
	
	// store the 8 direction cost for every point, if out of boundary, set the cost to -1
	for(i = 0; i < edge; i++) {
		for(j = 0; j < edge; j++) {
			for(compassPoint = 0; compassPoint < 8; compassPoint++) {
				directY = compass[compassPoint][0] + i;	// directY buffer the Y coordinate of the position detected
				directX = compass[compassPoint][1] + j;	// directX buffer the X coordinate of the position detected
				if(directY < 0 || directX < 0 || directY == edge || directX == edge) { // if out of boundary
					(*(terrainMap+i)+j)->cost[compassPoint] = -1;
				}
				else {
					(*(terrainMap+i)+j)->cost[compassPoint] = abs(initialMapData[i][j] - initialMapData[directY][directX]);
				}
			}
		}	
	}
	
	map->terrainMap = terrainMap;
	
	return map;
}


typedef struct queueUnit {
	short weight;
	short YX[2];
} QueueUnit;

typedef struct bfsMapUnit {
	byte isDestination;
	byte havePassed;
} BFSMapUnit;

void swapQueueUnit(QueueUnit*, QueueUnit*);

void setMapRouteCost(Map *map) {
	// declare variables for count
	short i, j, k;
	
	// malloc priorityQueue and BFSMap
	RAII_VARIABLE(QueueUnit*, priorityQueue, (QueueUnit*)malloc(((int)pow(map->edge, 2) + 1) * 4 * sizeof(QueueUnit)), free);
	RAII_VARIABLE(BFSMapUnit**, BFSMap, (BFSMapUnit**)malloc(map->edge * sizeof(BFSMapUnit*)), free);
	for(i = 0; i < map->edge; i++) {
		*(BFSMap+i) = (BFSMapUnit*)malloc(map->edge * sizeof(BFSMapUnit));
	}

	// specialPoint == start || destination
	short specialPoint[SpecialPointNum][2];
	specialPoint[0][0] = map->startAddress[0];
	specialPoint[0][1] = map->startAddress[1];
	for(i = 1; i <= map->destinationNum; i++) {
		specialPoint[i][0] = map->destination[i-1][0];
		specialPoint[i][1] = map->destination[i-1][1];
	}
	
	// mark isDestination on BFSMap
	for(i = 0; i < map->edge; i++) {
		for(j = 0; j < map->edge; j++) {
			BFSMap[i][j].isDestination = FALSE;
		}
	}
	for(i = 1; i <= map->destinationNum; i++) {
		BFSMap[specialPoint[i][0]][specialPoint[i][1]].isDestination = TRUE;
	}
	
	
	
	for(i = 0; i < map->destinationNum; i++) {
		// clean BFSMap by set havePassed to FALSE
		for(j = 0; j < map->edge; j++) {
			for(k = 0; k < map->edge; k++) {
				BFSMap[j][k].havePassed = FALSE;
			}
		}
		BFSMap[specialPoint[i][0]][specialPoint[i][1]].isDestination = FALSE;
		
		// initial priorityQueue
		short top = 1;
		QueueUnit *noteLeft, *noteRight;
		priorityQueue[top].weight = 0;
		priorityQueue[top].YX[0] = specialPoint[i][0];
		priorityQueue[top++].YX[1] = specialPoint[i][1];
		
		// initial specialPointNum
		short specialPointNum = map->destinationNum-i;
		
		// start dijkstra until having pass all specialPoints
		while(top != 1)	{
			// get the front of priorityQueue
			QueueUnit now = priorityQueue[1];
			
			// check whether it haveAlreadyPassed
			byte haveAlreadyPassed = FALSE;
			if(BFSMap[now.YX[0]][now.YX[1]].havePassed == FALSE) {	// if not havePassed
				BFSMap[now.YX[0]][now.YX[1]].havePassed = TRUE;
				
				// if it is Destination
				if(BFSMap[now.YX[0]][now.YX[1]].isDestination == TRUE) {
					specialPointNum--;
					for(j = i+1; j <= map->destinationNum; j++) {
						if(specialPoint[j][0] == now.YX[0] && specialPoint[j][1] == now.YX[1])
							break;
					}
					map->routeCost[i][j] = now.weight;
					map->routeCost[j][i] = now.weight;
				}
				if(specialPointNum == 0) {
					break;
				}
			}
			else {	// if havePassed
				haveAlreadyPassed = TRUE;
			}
			
			// dequeue() priorityQueue
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
			
			// if haveAlreadyPassed, continue
			if(haveAlreadyPassed) {
				continue;
			}
			
			// if not haveAlreadyPassed, try to addqueue(the point around now)
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

	// free lists in BFSMap
	for(i = 0; i < map->edge; i++) {
		free(*(BFSMap+i));
	}
}

void swapQueueUnit(QueueUnit *A, QueueUnit *B) {
	QueueUnit temp = *A;
	*A = *B;
	*B = temp;
}


// for record whether the point have passed for countLeastCost
byte destinationState[SpecialPointNum] = { FALSE };

void countLeastCost(Map *map, short state, short route) {
	// arrived this point
	destinationState[state] = TRUE;
	
	short i;
	
	// detect whether all points have passed
	byte fullFlag = TRUE;
	for(i = 1; i <= map->destinationNum; i++) {
		if(destinationState[i] == FALSE) {
			fullFlag = FALSE;
		}
	}
	
	// if all the special points have been passed
	if(fullFlag) {
		map->leastCost = (map->leastCost < route)? map->leastCost : route;
		destinationState[state] = FALSE;
		return;
	}
	
	// choose point to go from this state
	for(i = 1; i <= map->destinationNum; i++) {
		if(i == state) {
			continue;
		}
		if(destinationState[i] == FALSE) {
			countLeastCost(map, i, route + map->routeCost[state][i]);
		}
	}
	
	// leave this point
	destinationState[state] = FALSE;
}
