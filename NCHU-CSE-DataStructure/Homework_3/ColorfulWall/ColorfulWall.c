#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define INPUT_FILE "test_case_3-1/input_3.txt"
#define OUTPUT_FILE "output_3.txt"
#define TRUE 1

FILE *fin, *fout;

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
	fscanf(fin, "%d", &numOfInst);
	charBuff = fgetc(fin);
	
	char instBuff[40];
	int i;
	for(i = 0; i < numOfInst; i++) {
		fscanf(fin, "%[^\n]", instBuff);
		charBuff = fgetc(fin);
	}
	
	// close files
	fclose(fin);
	fclose(fout);
	
	return 0;
}
