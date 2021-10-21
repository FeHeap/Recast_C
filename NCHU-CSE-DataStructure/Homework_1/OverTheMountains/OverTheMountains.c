#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_FILE "input_3.txt"
#define OUTPUT_FILE "output_4.txt"
#define byte char

#define safeFree(p) saferFree((void**)&(p))


FILE *fin, *fout;

typedef struct mountain {
	short height;
	byte isDestination;
	int cost[8];
} Mountain;


typedef struct map {
	short destinationNum;
	short edge;
	short startAddress[2];
	short destination[5][2];
	Mountain **terrainMap;
} Map;


void saferFree(void**);
short** createInitailMap(short);
Map* createMap(short, short, short*, short(*)[2], short**);


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
		
		
		
		for(j = 0; j < edge; j++) {
			safeFree(*(aMap->terrainMap+j));
		}
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
	
	short **initialMapData = (byte**)malloc(edge * sizeof(short*));
	
	for(i = 0; i < edge; i++) {
		*(initialMapData+i) = (byte*)malloc(edge * sizeof(short));
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
	
	Mountain** terrainMap = (Mountain**)malloc(edge * sizeof(Mountain*));
	for(i = 0; i < edge; i++) {
		*(terrainMap+i) = (Mountain*)malloc(edge * sizeof(Mountain));
	}

	for(i = 0; i < edge; i++) {
		for(j = 0; j < edge; j++) {
			for(compassPoint = 0; compassPoint < 8; compassPoint++) {
				directY = compass[compassPoint][0] + i;	// directY buffer the Y coordinate of the position detected
				directX = compass[compassPoint][1] + j;	// directX buffer the X coordinate of the position detected
				if(directY < 0 || directX < 0 || directY == edge || directX == edge) { // if out of boundary, continue
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
