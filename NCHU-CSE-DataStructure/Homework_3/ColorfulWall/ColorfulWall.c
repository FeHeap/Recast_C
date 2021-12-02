#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#define INPUT_FILE "test_case_3-3/input_3.txt"
#define OUTPUT_FILE "test_case_3-3/output_3.txt"
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

void instructionProcess(BlocksManagement* , char*);

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
	int numOfBlocks = (wallLength % ((int)sqrt(wallLength)) == 0)? (int)sqrt(wallLength) : ((int)sqrt(wallLength) + 1);
	Block *blocks = (Block*)malloc(numOfBlocks * sizeof(Block));
	int i;
	for(i = 0; i < numOfBlocks; i++) {
		blocks[i].containToPrint = (char*)malloc(2 * numOfBlocks * sizeof(char));
		blocks[i].colors = (char*)malloc(numOfBlocks * sizeof(char));
	}
	
	for(i = 0; i < numOfBlocks; i++) {
		blocks[i].containToPrint[0] = '_';
		blocks[i].containToPrint[1] = '\0';
		blocks[i].colors[0] = '\0';
	}
	
	BlocksManagement BM;
	BM.wallLength = wallLength;
	BM.numOfBlocks = numOfBlocks;
	BM.blocks = blocks;
	
	fscanf(fin, "%d", &numOfInst);
	charBuff = fgetc(fin);
	char instBuff[30];
	for(i = 0; i < numOfInst; i++) {
		fscanf(fin, "%[^\n]", instBuff);
		//printf("%d %s\n", i, instBuff);
		charBuff = fgetc(fin);
		instructionProcess(&BM, instBuff);
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
			query(BM, &instruction[i]);
			break;
		case 'P':
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
	int left = strToInt(instruction);
	int  i = 0;
	while(!isspace(instruction[i++]));
	int right = strToInt(instruction + i);
	
	int numOfBlocks = BM->numOfBlocks;
	int leftBlock = left / numOfBlocks;
	int rightBlock = right / numOfBlocks;
	left %= numOfBlocks;
	right %= numOfBlocks;
	Block *blocks = BM->blocks;
	
	if(leftBlock == rightBlock) {
		if(blocks[leftBlock].colors[0] == '\0') {
			fprintf(fout, "%c\n", blocks[leftBlock].containToPrint[0]);
			//printf("%c\n", blocks[leftBlock].containToPrint[0]);
		}
		else {
			char *colors = blocks[leftBlock].colors;
			int colorState = colors[left];
			fprintf(fout, "%c", colorState);
			//printf("%c", colorState);
			int painter = 1;
			for(i = left; i <= right; i++) {
				if(colors[i] != colorState) {
					colorState = colors[i];
					fprintf(fout, " %c", colorState);
					//printf(" %c", colorState);
				}
			}
			fprintf(fout, "\n");
			//printf("\n");
		}
		
	}
	else {
		if(blocks[leftBlock].colors[0] == '\0')	{
			fprintf(fout, "%c", blocks[leftBlock].containToPrint[0]);
			//printf("%c", blocks[leftBlock].containToPrint[0]);
		}
		else {
			char *colors = blocks[leftBlock].colors;
			int colorState = colors[left];
			fprintf(fout, "%c", colorState);
			//printf("%c", colorState);
			for(i = left; i < numOfBlocks; i++) {
				if(colors[i] != colorState) {
					colorState = colors[i];
					fprintf(fout, " %c", colorState);
					//printf(" %c", colorState);
				}
			}
		}
		
		
		for(i = leftBlock + 1; i < rightBlock; i++) {
			if(blocks[i].containToPrint[0] == blocks[i-1].containToPrint[strlen(blocks[i-1].containToPrint)-1]) {
				if(blocks[i].containToPrint[1] != '\0')	{
					fprintf(fout, " %s", blocks[i].containToPrint + 2);
					//printf(" %s", blocks[i].containToPrint + 2);
				}
			}
			else {
				fprintf(fout, " %s", blocks[i].containToPrint);
				//printf(" %s", blocks[i].containToPrint);
			}
		}
		
		if(blocks[rightBlock].colors[0] == '\0') {
			if(blocks[rightBlock].containToPrint[0] != blocks[rightBlock-1].containToPrint[strlen(blocks[rightBlock-1].containToPrint)-1])	{
				fprintf(fout, " %c", blocks[rightBlock].containToPrint[0]);
				//printf(" %c", blocks[rightBlock].containToPrint[0]);
			}
		}
		else {
			char *colors = blocks[rightBlock].colors;
			int colorState = colors[0];
			if(blocks[rightBlock].containToPrint[0] != blocks[rightBlock-1].containToPrint[strlen(blocks[rightBlock-1].containToPrint)-1])	{
				fprintf(fout, " %c", colorState);
				//printf(" %c", colorState);
			}
			for(i = 0; i <= right; i++) {
				if(colors[i] != colorState) {
					colorState = colors[i];
					fprintf(fout, " %c", colorState);
					//printf(" %c", colorState);
				}
			}
		}
		
		fprintf(fout, "\n");
		//printf("\n");
	}
}

void paint(BlocksManagement *BM, char *instruction) {
	int left = strToInt(instruction);
	int  i = 0;
	while(!isspace(instruction[i++]));
	int right = strToInt(instruction + i);
	while(!isspace(instruction[i++]));
	char colorWillPrint = instruction[i];
	
	int numOfBlocks = BM->numOfBlocks;
	int leftBlock = left / numOfBlocks;
	int rightBlock = right / numOfBlocks;
	left %= numOfBlocks;
	right %= numOfBlocks;
	Block *blocks = BM->blocks;
	
	if(leftBlock == rightBlock) {
		
		int numOfColorBlock;
		if(rightBlock != numOfBlocks-1)	{
			numOfColorBlock = numOfBlocks;
		}
		else {
			numOfColorBlock = (BM->wallLength+1) % numOfBlocks;
		}
		
		
		char *colors = blocks[leftBlock].colors;
		char *containToPrint = blocks[leftBlock].containToPrint;
		
		if(colors[0] == '\0') {
			char onlyColor = containToPrint[0];
			for(i = 0; i < numOfColorBlock; i++) {
				colors[i] = onlyColor;
			}
		}
		
		for(i = left; i <= right ; i++) {
			colors[i] = colorWillPrint;
		}
		
		int colorState = colors[0];
		containToPrint[0] = colorState;
		int painter = 1;
		for(i = 1; i < numOfColorBlock; i++) {
			if(colors[i] != colorState) {
				colorState = colors[i];
				containToPrint[painter++] = ' ';
				containToPrint[painter++] = colorState;
			}
		}
		containToPrint[painter] = '\0';
		
	}
	else {
		for(i = leftBlock + 1; i < rightBlock; i++) {
			blocks[i].containToPrint[0] = colorWillPrint;
			blocks[i].containToPrint[1] = '\0';
			blocks[i].colors[0] = '\0';
		}
		
		// left
		char *colors = blocks[leftBlock].colors;
		char *containToPrint = blocks[leftBlock].containToPrint;
		if(colors[0] == '\0') {
			char onlyColor = containToPrint[0];
			for(i = 0; i < numOfBlocks; i++) {
				colors[i] = onlyColor;
			}
		}
		for(i = left; i < numOfBlocks; i++) {
			colors[i] = colorWillPrint;
		}
		int colorState = colors[0];
		containToPrint[0] = colorState;
		int painter = 1;
		for(i = 1; i < numOfBlocks; i++) {
			if(colors[i] != colorState) {
				colorState = colors[i];
				containToPrint[painter++] = ' ';
				containToPrint[painter++] = colorState;
			}
		}
		containToPrint[painter] = '\0';
		
		// right
		int numOfColorBlock;
		if(rightBlock != numOfBlocks-1)	{
			numOfColorBlock = numOfBlocks;
		}
		else {
			numOfColorBlock = (BM->wallLength+1) % numOfBlocks;
		}
		colors = blocks[rightBlock].colors;
		containToPrint = blocks[rightBlock].containToPrint;
		if(colors[0] == '\0') {
			char onlyColor = containToPrint[0];
			for(i = 0; i < numOfColorBlock; i++) {
				colors[i] = onlyColor;
			}
		}
		for(i = 0; i <= right; i++) {
			colors[i] = colorWillPrint;
		}
		colorState = colors[0];
		containToPrint[0] = colorState;
		painter = 1;
		for(i = 1; i < numOfColorBlock; i++) {
			if(colors[i] != colorState) {
				colorState = colors[i];
				containToPrint[painter++] = ' ';
				containToPrint[painter++] = colorState;	
			}
		}
		containToPrint[painter] = '\0';
	}
}

