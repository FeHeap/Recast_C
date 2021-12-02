#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#define INPUT_FILE "test_case_3-1/input_3.txt"
#define OUTPUT_FILE "output_3.txt"
#define TRUE 1

FILE *fin, *fout;

typedef struct block {
	int end;
	char *contain;
	int *indexOfColors;
} Block;

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
	fscanf(fin, "%d",&wallLength);
	charBuff = fgetc(fin);
	int numOfBlocks = (sqrt(wallLength) > (int)sqrt(wallLength))? ((int)sqrt(wallLength) + 1) : (int)sqrt(wallLength);
	Block *blocks = (Block*)malloc(numOfBlocks * sizeof(Block));
	int i;
	for(i = 0; i < numOfBlocks; i++) {
		blocks[i].end = (((i+1) * numOfBlocks) < wallLength)? ((i+1) * numOfBlocks) : wallLength;
		blocks[i].contain = (char*)malloc(2 * numOfBlocks * sizeof(char));
		blocks[i].indexOfColors = (int*)malloc((2 + numOfBlocks) * sizeof(int));
	}
	
	strcpy(blocks[0].contain, "_");
	blocks[0].indexOfColors[0] = 0;
	blocks[0].indexOfColors[i] = numOfBlocks;
	for(i = 1; i < numOfBlocks; i++) {
		if(indexOfColors[0] < wallLength) {
			strcpy(blocks[i].contain, "_");
		}
		else {
			blocks[i].contain[0] = '\0';
		}
		blocks[i].indexOfColors[0] = blocks[i-1].indexOfColors[1];
		blocks[i].indexOfColors[1] = blocks[i].end;
	}
	
	fscanf(fin, "%d", &numOfInst);
	charBuff = fgetc(fin);
	
	char instBuff[40];
	for(i = 0; i < numOfInst; i++) {
		fscanf(fin, "%[^\n]", instBuff);
		charBuff = fgetc(fin);
	}
	
	
	for(i = 0; i < numOfBlocks; i++) {
		free(blocks[i].contain);
		free(blocks[i].indexOfColors);
	}
	free(blocks);
	
	// close files
	fclose(fin);
	fclose(fout);
	
	return 0;
}

void query(Block*, int, char*);
void paint(Block*, int, char*);

void instructionProcess(Block* blocks, int numOfBlocks, char *instruction) {
	int  i = 0;
	while(!isspace(instruction[i++]));
	switch(instruction[0]) {
		case 'Q':
			//printf("Q\n");
			query(blocks, numOfBlocks, &instruction[i]);
			break;
		case 'P':
			//printf("P\n");
			paint(blocks, numOfBlocks, &instruction[i]);
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

void query(Block* blocks, int numOfBlocks, char *instruction) {
	
}

void paint(Block* blocks, int numOfBlocks, char *instruction) {
	int left = strToInt(instruction);
	int  i = 0;
	while(!isspace(instruction[i++]));
	int right = strToInt(instruction + i);
	while(!isspace(instruction[i++]));
	char color = instruction[i];
	
	int leftBlock = left / numOfBlocks;
	int rightBlock = right / numOfBlocks;
	
	if(leftBlock == rightBlock) {
		
	}
	else {
		for(i = leftBlock + 1; i < rightBlock; i++) {
			blocks[i].contain[0] = color;
			blocks[i].contain[1] = '\0';
			blocks[i].indexOfColors[1] = blocks[i].end;
		}
	}
}


