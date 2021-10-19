#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "input_2.txt"
#define OUTPUT_FILE "output_2.txt"
#define byte char

#define safeFree(p) saferFree((void**)&(p))


FILE *fin, *fout;

void saferFree(void**);

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
	
	// map data container
	unsigned int edge, generations;
	byte **map, **mapBuff, **temp;
	
	// compass to detect 8 directions around
	short compass[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1},};
	short directY, directX;
	byte compassPoint;	// point of compass
	byte around;	// record the number of cells serviving around a position
	
	
	unsigned int mapNum;	// the number of maps
	char charBuff;			// to buff blank or '\n'
	fscanf(fin, "%u", &mapNum);	// get the number of maps
	charBuff = fgetc(fin);	// buff '\n'
	
	printf("Comversing data to sparse matrices...\n");
	
	// variable to count the times
	short i, j, k, g;
	// which map do the process read
	for(i = 0; i < mapNum; i++) {	// map
		// get the edge of map
		fscanf(fin, "%u", &edge);
		charBuff = fgetc(fin);	// buff blank
		//  get the generations
		fscanf(fin, "%u", &generations);
		charBuff = fgetc(fin);	// buff '\n'
		
		// create a map
		map = (byte**)malloc(edge * sizeof(byte*));
		for(j = 0; j < edge; j++) {
			*(map+j) = (byte*)malloc(edge * sizeof(byte));
		}
		// create a map buffer
		mapBuff = (byte**)malloc(edge * sizeof(byte*));
		for(j = 0; j < edge; j++) {
			*(mapBuff+j) = (byte*)malloc(edge * sizeof(byte));
		}
		
		// read the data of the map in INPUT_FILE and store it into variable map in the process
		for(j = 0; j < edge; j++) {		// Y
			for(k = 0; k < edge; k++) {	// X
				fscanf(fin, "%hhu", (*(map+j)+k));	// whether the cell servive
				charBuff = fgetc(fin);	// buff blank or '\n'
			}		
		}
		
		// generational change
		for(g = 0; g < generations; g++) { 		// generations
			for(j = 0; j < edge; j++) {			// Y
				for(k = 0; k < edge; k++) { 	// X
					// initialize variable around to zero
					around = 0;
					
					// detect around with the order of direction in compass
					for(compassPoint = 0; compassPoint < 8; compassPoint++) {
						directY = compass[compassPoint][0] + j;	// directY buffer the Y coordinate of the position detected
						directX = compass[compassPoint][1] + k;	// directX buffer the X coordinate of the position detected
						if(directY < 0 || directX < 0 || directY == edge || directX == edge) { // if out of boundary, continue
							continue;
						}
						if(*(*(map+directY)+directX) == 1) {
							around++;
						}
					}
					
					// deceide what state the position map[j][k] will be next generation and store it into mapBuff[j][k] temporarily
					if(*(*(map+j)+k) == 0) {
						*(*(mapBuff+j)+k) = (around == 3)? 1 : 0;
					}
					else {
						*(*(mapBuff+j)+k) = (around == 2 || around == 3)? 1 : 0;
					}
				}
			}
			
			// swap(map, mapBuff)	change generation
			temp = map;
			map = mapBuff;
			mapBuff = temp;
		}
		
		// free mapBuff
		for(j = 0; j < edge; j++) {
			safeFree(*(mapBuff+j));
		}
		safeFree(mapBuff);
		
		// count the number of cells surviving
		short cells = 0;
		for(j = 0; j < edge; j++) {
			for(k = 0; k < edge; k++) {
				if(*(*(map+j)+k) == 1) {
					cells++;
				}
			}
		}
		
		// fout: rows, cols, number of non-zero items
		fprintf(fout, "%hd %hd %hd\n", edge, edge, cells);
		for(j = 0; j < edge; j++) {			// Y
			for(k = 0; k < edge; k++) {		// X
				if(*(*(map+j)+k) == 1) {
					// initialize variable around to zero
					around = 0;
					// detect around with the order of direction in compass
					for(compassPoint = 0; compassPoint < 8; compassPoint++) {
						directY = compass[compassPoint][0] + j;	// directY buffer the Y coordinate of the position detected
						directX = compass[compassPoint][1] + k;	// directX buffer the X coordinate of the position detected
						if(directY < 0 || directX < 0 || directY == edge || directX == edge) { // if out of boundary, continue
							continue;
						}
						if(*(*(map+directY)+directX) == 1) {
							around++;
						}
					}
					// fout: row, col, number of surviving neighbors
					fprintf(fout, "%hd %hd %hd\n", j, k, around);
				}
			}
		}
		
		// free map
		for(j = 0; j < edge; j++) {
			safeFree(*(map+j));
		}
		safeFree(map);
	}
	// end of fout
	fprintf(fout, "\n\n\n", j, k, around);
	
	// close files
	fclose(fin);
	fclose(fout);
	
	printf("Successfully generate the sparse matrices and store they into %s\n", OUTPUT_FILE);
	
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
