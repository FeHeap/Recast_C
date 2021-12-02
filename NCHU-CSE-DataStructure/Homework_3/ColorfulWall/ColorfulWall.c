#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#define INPUT_FILE "test_case_3-1/input_3.txt"
#define OUTPUT_FILE "output_3.txt"
#define TRUE 1

FILE *fin, *fout;



typedef struct block {
	char *containToPrint;
	char *colors;
} Block;

typedef struct blocksManagement {
	int wallLength;
	int numOfBlocks;
	Block *blocks;
} BlocksManagement;

void instructionProcess(Block*, int, char*);

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
	
	
	
	int wallLength;
	int numOfInst;
	char charBuff;
	fscanf(fin, "%d", &wallLength);
	charBuff = fgetc(fin);
	int numOfBlocks = (sqrt(wallLength) > (int)sqrt(wallLength))? ((int)sqrt(wallLength) + 1) : (int)sqrt(wallLength);
	Block *blocks = (Block*)malloc(numOfBlocks * sizeof(Block));
	int i;
	for(i = 0; i < numOfBlocks; i++) {
		blocks[i].containToPrint = (char*)malloc(2 * numOfBlocks * sizeof(char));
		blocks[i].colors = (int*)malloc(numOfBlocks * sizeof(char));
	}
	
	for(i = 0; i < numOfBlocks; i++) {
		if(i * numOfBlocks < wallLength) {
			blocks[i].containToPrint[0] = '_';
			blocks[i].containToPrint[1] = '\0';
			blocks[i].colors[0] = '\0';
		}
		else {
			blocks[i].containToPrint[0] = '\0';
		}
	}
	
	BlocksManagement BM;
	BM.wallLength = wallLength;
	BM.numOfBlocks = numOfBlocks;
	BM.blocks = blocks;
	
	fscanf(fin, "%d", &numOfInst);
	charBuff = fgetc(fin);
	
	char instBuff[40];
	for(i = 0; i < numOfInst; i++) {
		fscanf(fin, "%[^\n]", instBuff);
		charBuff = fgetc(fin);
	}
	
	
	for(i = 0; i < numOfBlocks; i++) {
		free(blocks[i].containToPrint);
		free(blocks[i].colors);
	}
	free(blocks);
	blocks = NULL;
	BM.blocks = NULL;
	
	// close files
	fclose(fin);
	fclose(fout);
	
	return 0;
}

void query(BlocksManagement*, char*);
void paint(BlocksManagement*, char*);

void instructionProcess(BlocksManagement *BM, char *instruction) {
	int  i = 0;
	while(!isspace(instruction[i++]));
	switch(instruction[0]) {
		case 'Q':
			//printf("Q\n");
			query(BM, &instruction[i]);
			break;
		case 'P':
			//printf("P\n");
			paint(BM, &instruction[i]);
			break;
		default: // pass
			break;
	}
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

void query(BlocksManagement *BM, char *instruction) {
	
}

void paint(BlocksManagement *BM, char *instruction) {
	int left = strToInt(instruction);
	int  i = 0;
	while(!isspace(instruction[i++]));
	int right = strToInt(instruction + i);
	while(!isspace(instruction[i++]));
	char color = instruction[i];
	
	int leftBlock = left / numOfBlocks;
	int rightBlock = right / numOfBlocks;
	
	if(leftBlock == rightBlock) {
		if(leftBlock == )
		if(blocks[leftBlock].colors[0] == '\0') {
			
			for(i = 0; i < numOfBlocks; i++) {
				
			}
		}
		
	}
	
	for(i = leftBlock + 1; i < rightBlock; i++) {
		blocks[i].containToPrint[0] = color;
		blocks[i].containToPrint[1] = '\0';
		blocks[i].colors[0] = '\0';
	}
	
}

