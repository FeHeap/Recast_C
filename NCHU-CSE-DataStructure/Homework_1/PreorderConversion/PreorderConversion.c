#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define INPUT_FILE "input_1_2.txt"
#define OUTPUT_FILE "output_1_2.txt"
#define TRUE 1
#define byte char

#define safeFree(p) saferFree((void**)&(p))

FILE *fin, *fout;

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
void PreorderConverse(Unit**);

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
	
	// declare buffers
	char FormulaBuff[101];
	char lnBuff;
	
	// count the number of formulas
	size_t lines = 0;
	do {
		fscanf(fin, "%[^\n]", FormulaBuff);
		lnBuff = fgetc(fin);	// buff '\n'
		lines++;	
	} while(lnBuff != EOF);
	rewind(fin);
	
	printf("Comversing inorder formulas to preorder...\n");
	
	// read the formulas in INPUT_FILE, and write the preorder of formulas in OUTPUT_FILE
	int i;
	for(i = 0; i < lines; i++) {
		// initialize FormulaBuff
		FormulaBuff[0] = '\0';
		
		// read the formula in INPUT_FILE, and store it into FormulaBuff
		fscanf(fin, "%[^\n]", FormulaBuff);
		lnBuff = fgetc(fin);	// buff '\n'
		
		// if space line, fprintf '\n'
		if(strlen(FormulaBuff) == 0) {
			fprintf(fout, "\n");
			continue;
		}
		
		// converse the data in FormulaBuff to linked list(stack, head point to the top of stack)
		Unit *Head = structUpFormula(FormulaBuff);
		
		// converse inorder formula stored in the linked list to preorder formula
		PreorderConverse(&Head);
		
		// write the preorder formula into OUTPUT_FILE
		Unit *tempUnit = Head;
		Head = Head->next;
		if(tempUnit->type == type_operate) {
			fprintf(fout, "%c", tempUnit->content[0]); 
		}
		else {	// if(tempUnit->type == type_num)
			fprintf(fout, "%s", tempUnit->content);
		}
		safeFree(tempUnit->content);
		safeFree(tempUnit);
		while(Head != NULL) {
			Unit *tempUnit = Head;
			Head = Head->next;
			if(tempUnit->type == type_operate) {
				fprintf(fout, " %c", tempUnit->content[0]); 
			}
			else {	// if(tempUnit->type == type_num)
				fprintf(fout, " %s", tempUnit->content);
			}
			safeFree(tempUnit->content);
			safeFree(tempUnit);
		}
		fprintf(fout, "\n");
	}
	
	
	// close files
	fclose(fin);
	fclose(fout);
	
	printf("Successful conversion and store the outcome into %s\n", OUTPUT_FILE);
	
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

// states of NFA_PreorderConversion_rough
enum state {
	init,			// 0
	openParen,		// 1
	num,			// 2
	operate,		// 3
	closeParen,		// 4
	end,			// 5
};

void pushFormulaStruct(Unit**, const char*);

Unit* structUpFormula(const char *Formula) {
	
	size_t length = strlen(Formula);
	
	// declare buffers
	char numBuff[101];
	char operateBuff[2] = {'\0', '\0'};
	
	// build the linked list to store Formula by NFA_PreorderConversion_rough
	Unit *head = NULL;
	enum state point = init;	// point to the start state
	int i, j;
	for(i = 0; i < length; i++) {
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


Unit* popFormulaStruct(Unit**);
byte getOperatorWeight(char);

void PreorderConverse(Unit** formulaHead) {
	
	Unit *preorderFormula = NULL;
	Unit *symbolStackTop = NULL;
	Unit *UnitBuff = NULL;
	Unit *tempUnit = NULL;
	
	while(*formulaHead != NULL) {
		// get the top of *formulaHead
		UnitBuff = popFormulaStruct(formulaHead);
	
		// process the UnitBuff
		if(UnitBuff->type == type_num) {
			UnitBuff->next = preorderFormula;
			preorderFormula = UnitBuff;
		}
		else if(UnitBuff->type == type_operate) {
			if(symbolStackTop != NULL && symbolStackTop->type != type_close) {
				// modify the symbolStackTop by the weight of operators
				while(getOperatorWeight(UnitBuff->content[0]) > getOperatorWeight(symbolStackTop->content[0])) {
					tempUnit = symbolStackTop;
					symbolStackTop = symbolStackTop->next;
					tempUnit->next = preorderFormula;
					preorderFormula = tempUnit;
					if(symbolStackTop == NULL || symbolStackTop->type == type_close) {
						break;
					}
				}
			}
			UnitBuff->next = symbolStackTop;
			symbolStackTop = UnitBuff;
		}
		else if(UnitBuff->type == type_close) {
			UnitBuff->next = symbolStackTop;
			symbolStackTop = UnitBuff;
		}
		else {	// if(UnitBuff->type == type_open)
			safeFree(UnitBuff);	// free open paren
			while(symbolStackTop->type != type_close) {
				tempUnit = symbolStackTop;
				symbolStackTop = symbolStackTop->next;
				tempUnit->next = preorderFormula;
				preorderFormula = tempUnit;
			}
			UnitBuff = symbolStackTop;
			symbolStackTop = symbolStackTop->next;
			safeFree(UnitBuff);	// free close paren
		}
	}
	
	// pop the remaining elements in symbolStackTop, and push it into preorderFormula
	while(symbolStackTop != NULL) {
		Unit *tempUnit = symbolStackTop;
		symbolStackTop = symbolStackTop->next;
		tempUnit->next = preorderFormula;
		preorderFormula = tempUnit;
	}
	
	// point *formulaHead to preorderFormula
	*formulaHead = preorderFormula;
}

void pushFormulaStruct(Unit **top, const char *content) {
	
	Unit *tempUnit = (Unit*)malloc(sizeof(Unit));
	
	if(strlen(content) == 1 && !isalnum(content[0])) {
		if(content[0] == '(') {
			tempUnit->type = type_open;
			tempUnit->content = NULL;
		}
		else if(content[0] == ')') {
			tempUnit->type = type_close;
			tempUnit->content = NULL;
		}
		else {
			tempUnit->type = type_operate;
			tempUnit->content = (char*)malloc(sizeof(char));
			tempUnit->content[0] = content[0];
		}
	}
	else {
		tempUnit->type = type_num;
		tempUnit->content = (char*)malloc((strlen(content) + 1) * sizeof(char));
		strcpy(tempUnit->content, content);
	}
	
	tempUnit->next = *top;
	*top = tempUnit;
}

Unit* popFormulaStruct(Unit **top) {
	Unit *tempUnit = *top;
	*top = (*top)->next;
	return tempUnit;
}

byte getOperatorWeight(char Operator) {
	
	byte weight;
	
	switch(Operator) {
		case '*':
		case '/':
		case '%':
			weight = 0;
			break;
			
		case '+':
		case '-':
			weight = 1;
			break;
			
		case '>':
		case '<':
			weight = 2;
			break;
			
		case '&':
			weight = 3;
			break;
		case '^':
			weight = 4;
			break;
		case '|':
			weight = 5;
			break;
			
		default: //pass
			break;
	}
	
	return weight;
}

