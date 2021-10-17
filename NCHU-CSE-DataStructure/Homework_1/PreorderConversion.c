#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_FILE "input_1.txt"
#define OUTPUT_FILE "output_1.txt"
#define TRUE 1
#define FALSE 0
typedef char bool;


FILE *fin, *fout;

typedef struct node {
	char value[30];
	struct node *left;
	struct node *right;
} Node;

Node* buildTree(char*);

char* preorderConverse(char*, int*, int);

int main() {
		
	if((fin = fopen(INPUT_FILE, "r")) == NULL) {
		printf("Fail to open file %s!", INPUT_FILE);
		exit(EXIT_FAILURE);
	}
	
//	if(!((fout = fopen(OUTPUT_FILE, "r")) == NULL)) {
//		fclose(fin);
//		printf("File %s has already existed! Overwrite?[Y/n]", OUTPUT_FILE);
//		char Overwrite = 'n';
//		scanf(" %c", &Overwrite);
//		if(!(Overwrite == 'y' || Overwrite == 'Y')) {
//			exit(EXIT_SUCCESS);
//		}
//	}
//	fout = fopen(OUTPUT_FILE, "w");
	
	
	
	char FormulaBuff[101];
	char lnBuff;
	

	fscanf(fin, "%[^\n]", FormulaBuff);
	lnBuff = fgetc(fin);	// \n
	int start = 0;
	char *outcome = buildTree(FormulaBuff);
	printf("main\n");	
	printf("main:%s\n", outcome);
	free(outcome);
	outcome = NULL;
	
//	fscanf(fin, "%[^\n]", FormulaBuff);
//	lnBuff = fgetc(fin);	// \n
//	start = 0;
//	outcome = preorderConverse(FormulaBuff, &start, 0);
//	printf("main:%s\n", outcome);
//	free(outcome);
//	outcome = NULL;

	
	
	fclose(fin);
	//fclose(fout);
	system("PAUSE");
	return 0;
}

enum state {
	openParenthesis,	//0
	closeParenthesis,	//1
	num,				//2
	operate,			//3
	end,				//4
};

enum operatorPriority {
	first = 4,	// * % /	
	second = 3,	// + -
	third = 2,	// > <
	fourth = 1,	// & ^ |
	fifth = 0,	// nothing
};

bool isOperator(char);
bool isOpenParenthesis(char);
bool isCloseParenthesis(char);
int getOperatorPriority(char);





bool isOperator(char character) {
	
	bool result;
	switch(character) {
		case '*':
		case '/':
		case '%':
		case '+':
		case '-':
		case '>':
		case '<':
		case '&':
		case '^':
		case '|':
			result = TRUE;
			break;
		default:
			result = FALSE;
			break;
	}
	
	return result;
}

bool isOpenParenthesis(char character) {
	return (character == '(')? TRUE : FALSE;
}

bool isCloseParenthesis(char character) {
	return (character == ')' || character == '\0')? TRUE : FALSE;
}

int getOperatorPriority(char Operator) {
	
	enum operatorPriority priority;
	
	switch(Operator) {
		case '*':
		case '/':
		case '%':
			priority = first;
			break;
			
		case '+':
		case '-':
			priority = second;
			break;
			
		case '>':
		case '<':
			priority = third;
			break;
			
		case '&':
		case '^':
		case '|':
			priority = fourth;
			break;
			
		default:
			priority = fifth;
			break;
	}
	
	return priority;
}
