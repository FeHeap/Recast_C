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
	} type;
	char *content;
	struct unit *next;
} Unit;


void saferFree(void**);
Unit* structUpFormula(const char*);
void pushFormulaStruct(Unit**, const char*);
Unit* popFormulaStruct(Unit**);
bool isOperator(char);
byte getOperatorPriority(char);
void PreorderConverse(Unit**);

int main() {
	
	
	// output file opened with mode "w"
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
	fout = fopen(OUTPUT_FILE, "w");
	
	char FormulaBuff[101];
	char lnBuff;
	
	// count the number of formulas
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
		PreorderConverse(&Head);
		
		Unit *temp = Head;
		Head = Head->next;
		if(temp->type == type_operate)
			fprintf(fout, "%c", temp->content[0]);
		else if(temp->type == type_num)
			fprintf(fout, "%s", temp->content);
		safeFree(temp->content);
		safeFree(temp);
		while(Head != NULL) {
			Unit *temp = Head;
			Head = Head->next;
			if(temp->type == type_operate)
				fprintf(fout, " %c", temp->content[0]);
			else if(temp->type == type_num)
				fprintf(fout, " %s", temp->content);
			safeFree(temp->content);
			safeFree(temp);
		}
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n\n");
	
	// close files
	fclose(fin);
	fclose(fout);
	
	
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


// states of NFA_PreorderConversion_rough
enum state {
	init,			// 0
	openParen,		// 1
	num,			// 2
	operate,		// 3
	closeParen,		// 4
	end,			// 5
};

Unit* structUpFormula(const char *Formula) {
	
	Unit *head = NULL;
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
				// task
				for(j = 0; TRUE; i++) {
					numBuff[j++] = Formula[i];
					if(!isalnum(Formula[i+1]) && Formula[i+1] != '.') {
						break;
					}
				}
				numBuff[j] = '\0';
				pushFormulaStruct(&head, numBuff);
				
				// transition
				if(Formula[i+1] == ')') {
					point = closeParen;
				}
				else if(Formula[i+1] == '\0') {
					point = end;
				}
				else { // Formula[i+1] is operator
					point = operate;
				}
				break;
				
			case operate:
				// task
				operateBuff[0] = Formula[i];
				pushFormulaStruct(&head, operateBuff);
				
				// transition
				if(isalnum(Formula[i+1]) || Formula[i+1] == '-') {
					point = num;
				}
				else {	// Formula[i+1] == '('
					point = openParen;
				}
				break;
				
			case openParen:
				// task
				pushFormulaStruct(&head, "(");
				
				// transition
				if(isalnum(Formula[i+1]) || Formula[i+1] == '-'){
					point = num;
				}
				break;
				
			case closeParen:
				// task
				pushFormulaStruct(&head, ")");
				
				// transition
				if(Formula[i+1] == '\0') {
					point = end;
				}
				else if(Formula[i+1] != ')'){ // Formula[i+1] is operator
					point = operate;
				}
				break;
				
			case init:
				// transition
				point = (isalnum(Formula[i]) || Formula[i] == '-')? num : openParen;
				i--; 
				break;
				
			default: // pass
				break;
		}
	}
	
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

Unit* popFormulaStruct(Unit **top) {
	Unit *temp = *top;
	*top = (*top)->next;
	return temp;
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
			
		default: //pass
			break;
	}
	
	return priority;
}


void PreorderConverse(Unit** formulaHead) {
	Unit *preorderFormula = NULL;
	Unit *symbolStackTop = NULL;
	
	
	while(*formulaHead != NULL) {
		Unit *tempUnit = popFormulaStruct(formulaHead);
		if(tempUnit->type == type_operate || tempUnit->type == type_open || tempUnit->type == type_close) {
			if(tempUnit->type == type_operate) {
				if(symbolStackTop != NULL && symbolStackTop->type != type_close) {
					while(getOperatorWeight(tempUnit->content[0]) > getOperatorWeight(symbolStackTop->content[0])) {
						Unit *temp = symbolStackTop;
						symbolStackTop = symbolStackTop->next;
						temp->next = preorderFormula;
						preorderFormula = temp;
						if(symbolStackTop == NULL || symbolStackTop->type == type_close) {
							break;
						}
					}
					tempUnit->next = symbolStackTop;
					symbolStackTop = tempUnit;
				}
				else {
					tempUnit->next = symbolStackTop;
					symbolStackTop = tempUnit;
				}
			}
			else if(tempUnit->type == type_close) {
					tempUnit->next = symbolStackTop;
					symbolStackTop = tempUnit;
			}
			else {
				safeFree(tempUnit);
				while(symbolStackTop->type != type_close) {
					Unit *temp = symbolStackTop;
					symbolStackTop = symbolStackTop->next;
					temp->next = preorderFormula;
					preorderFormula = temp;
				}
				tempUnit = symbolStackTop;
				symbolStackTop = symbolStackTop->next;
				safeFree(tempUnit);
			}
		}
		else {	// tempUnit->type == type_num
			tempUnit->next = preorderFormula;
			preorderFormula = tempUnit;
		}
	}
	
	while(symbolStackTop != NULL) {
		Unit *temp = symbolStackTop;
		symbolStackTop = symbolStackTop->next;
		temp->next = preorderFormula;
		preorderFormula = temp;
	}
	
	*formulaHead = preorderFormula;
}
