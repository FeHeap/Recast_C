#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define INPUT_FILE "input_1.txt"
#define OUTPUT_FILE "output_1.txt"
#define TRUE 1
#define FALSE 0
#define byte char

#define safeFree(p) saferFree((void**)&(p))



FILE *fin, *fout;

typedef char bool;


typedef struct unit {
	enum unitType {
		type_num,
		type_operate,
		type_open,
		type_close,
		type_head,
	} type;
	char *content;
	struct unit *next;
} Unit;

void saferFree(void**);
Unit* structUpFormula(const char*);
Unit* initHeadUnit();
void pushFormulaStruct(Unit**, const char*);
char* preorderConverse(char*, int*, int);
bool isOperator(char);
byte getOperatorPriority(char);

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
	size_t lines = 0;
	do {
		fscanf(fin, "%[^\n]", FormulaBuff);
		lnBuff = fgetc(fin);	// \n
		lines++;	
	} while(lnBuff != EOF);
	lines -= 2;
	rewind(fin);
	int i;
	for(i = 0; i < lines; i++) {
		fscanf(fin, "%[^\n]", FormulaBuff);
		lnBuff = fgetc(fin);	// \n
		Unit *Head = structUpFormula(FormulaBuff);
		while(Head != NULL) {
			Unit *temp = Head;
			Head = Head->next;
			if(temp->type == type_operate)
				printf("%c", temp->content[0]);
			else if(temp->type == type_num)
				printf("%s", temp->content);
			else if(temp->type == type_open)
				printf("(");
			else if(temp->type == type_close)
				printf(")");
			safeFree(temp->content);
			safeFree(temp);
		}
		printf("\n");
	}
	
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

void saferFree(void **pp){
	//printf("SAFER FREE\n");
	if(pp != NULL && *pp != NULL){
		free(*pp);
		*pp = NULL;
	}
}

enum state {
	init,		// 0
	open,		// 1
	num,		// 2
	operate,	// 3
	close,		// 4
	end,		// 5
};

Unit* structUpFormula(const char *Formula) {
	
	Unit *head = initHeadUnit();
	size_t length = strlen(Formula);
	char numBuff[101];
	char operateBuff[2];
	operateBuff[1] = '\0';
	enum state point = init;
	int i, j;
	for(i = 0; i < length; i++) {
		// NFA_PreorderConversion_rough
		switch(point) {
			case num:
				for(j = 0; TRUE; i++) {
					numBuff[j++] = Formula[i];
					if(!isalnum(Formula[i+1]) && Formula[i+1] != '.') {
						break;
					}
				}
				numBuff[j] = '\0';
				pushFormulaStruct(&head, numBuff);
				if(Formula[i+1] == ')') {
					point = close;
				}
				else if(Formula[i+1] == '\0') {
					point = end;
				}
				else { // Formula[i+1] is operator
					point = operate;
				}
				break;
				
			case operate:
				operateBuff[0] = Formula[i];
				pushFormulaStruct(&head, operateBuff);
				if(isalnum(Formula[i+1]) || Formula[i+1] == '-') {
					point = num;
				}
				else {	// Formula[i+1] == '('
					point = open;
				}
				break;
				
			case open:
				pushFormulaStruct(&head, "(");
				if(isalnum(Formula[i+1]) || Formula[i+1] == '-'){
					point = num;
				}
				break;
				
			case close:
				pushFormulaStruct(&head, ")");
				if(Formula[i+1] == '\0') {
					point = end;
				}
				else if(Formula[i+1] != ')'){ // Formula[i+1] is operator
					point = operate;
				}
				break;
				
			case init:
				point = (isalnum(Formula[i]) || Formula[i] == '-')? num : open;
				i--; 
				break;
				
			default:
				// pass
				break;
		}
	}
	
	return head;
}

Unit* initHeadUnit() {
	
	Unit *head  = (Unit*)malloc(sizeof(Unit));
	head->content = NULL;
	head->type = type_head;
	head->next = NULL;
	
	return head;
}

void pushFormulaStruct(Unit **top, const char *content) {
	
	Unit *temp = (Unit*)malloc(sizeof(Unit));
	
	if(strlen(content) == 1 && !isalnum(content[0])) {
		if(content[0] == '(') {
			temp->type = type_open;
			temp->content = NULL;
		}
		else if(content[0] == ')') {
			temp->type = type_close;
			temp->content = NULL;
		}
		else {
			temp->type = type_operate;
			temp->content = (char*)malloc(sizeof(char));
			temp->content[0] = content[0];
		}
	}
	else {
		temp->type = type_num;
		temp->content = (char*)malloc((strlen(content) + 1) * sizeof(char));
		strcpy(temp->content, content);
	}
	
	temp->next = *top;
	*top = temp;
}

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


byte getOperatorWeight(char Operator) {
	
	byte priority;
	
	switch(Operator) {
		case '*':
		case '/':
		case '%':
			priority = 0;
			break;
			
		case '+':
		case '-':
			priority = 1;
			break;
			
		case '>':
		case '<':
			priority = 2;
			break;
			
		case '&':
		case '^':
		case '|':
			priority = 3;
			break;
			
		default:
			//pass
			break;
	}
	
	return priority;
}
