#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define inputBufferSize 100

typedef struct polynomial polynomial;
struct polynomial {
	int coefficient;
	int exponent;
	polynomial *next;
};

polynomial* inputPolynomial(char*);
int isValid(char*);
int isExit(char*);
char getOperationType(char*);
char inputOperationSymbol(char*);
polynomial* operation(char, polynomial*, polynomial*);
polynomial* addition(polynomial*, polynomial*);
polynomial* subtraction(polynomial*, polynomial*);
polynomial* multiplication(polynomial*, polynomial*);
polynomial* stringToPolynomial(char*);
void push(polynomial* , int, int);
char* polynomialToString(polynomial*);

int main() {
	printf("Input polynomials and operation symbol('*'¡B'-'¡B'+')\n");
	polynomial *p1 = inputPolynomial("Polynomial 1:");
	polynomial *p2 = inputPolynomial("Polynomial 2:");
	char operationSymbol = inputOperationSymbol("Operation Symbol:");
	polynomial* ans = operation(operationSymbol, p1, p2);
	printf("Ans: %s\n", polynomialToString(ans));
	
	system("PAUSE");
	return 0;
}

polynomial* inputPolynomial(char* hint) {
	char input[inputBufferSize];
	do {
		printf("%s", hint);
		gets(input);
		if(isExit(input)) {
			printf("bye~\n");
			exit(0);
		}
	}while(!isValid(input));
	
	return stringToPolynomial(input);
}

enum state {
	start,
	sign,
	coef,
	x,
	caret,
	expo,
	operate,
	dead
} point;

int isValid(char* polynomialStr) {
	int len = strlen(polynomialStr);
	int i;
	char charBuff;
	point = start;
	for(i = 0; i < len; i++) {
		if(isspace(polynomialStr[i])) {
			continue;
		}
		
		charBuff = polynomialStr[i];
		switch (point) {
			case coef:
				if(isdigit(charBuff)) {
					//pass
				} 
				else if(charBuff == 'x' || charBuff == 'X') {
					point = x;
				}
				else {
					point = dead;	
				}
				break;
				
			case x:
				if(charBuff == '^') {
					point = caret;
				}
				else if(charBuff == '+' || charBuff == '-') {
					point = operate;
				}
				else {
					point = dead;
				}
				break;
				
			case caret:
				if(isdigit(charBuff)) {
					point = expo;
				}
				else {
					point = dead;
				}
				break;
				
			case expo:
				if(isdigit(charBuff)) {
					//pass
				}
				else if(charBuff == '+' || charBuff == '-') {
					point = operate;
				}
				else {
					point = dead;
				}
				break;
				
			case operate:
				if(isdigit(charBuff)) {
					point = coef;
				}
				else if(charBuff == 'x' || charBuff == 'X') {
					point = x;
				}
				else {
					point = dead;
				}
				break;
			
			case start:
				if(isdigit(charBuff)) {
					point = coef;
				} 
				else if(charBuff == 'x' || charBuff == 'X') {
					point = x;
				}
				else if(charBuff == '+' || charBuff == '-') {
					point = sign;
				}
				else {
					point = dead;	
				}
				break;
				
			case sign:
				if(isdigit(charBuff)) {
					point = coef;
				} 
				else if(charBuff == 'x' || charBuff == 'X') {
					point = x;
				}
				else {
					point = dead;
				}
				break;
		}
		
		if(point == dead) {
			break;
		}
	}
	
	return ( point == start || point == coef || point == x || point == expo );
}

int isExit(char* str) {
	int flag = 0;
	int len = strlen(str);
	int i;
	for(i = 0; i < len; i++) {
		if(isspace(str[i])) {
			continue;
		}
		
		if(flag == 0 && (str[i] == 'e' || str[i] == 'E')) {	
			if((str[i+1] == 'x' || str[i+1] == 'X') && (str[i+2] == 'i' || str[i+2] == 'I') && (str[i+3] == 't' || str[i+3] == 'T')) {
				flag = 1;
				i += 3;
			}
			else {
				break;
			}
		}
		else {
			flag = 0;
			break;
		}
	}
	
	return flag;
}

char inputOperationSymbol(char* hint) {
	char input[inputBufferSize];
	char operationType;
	do {
		printf("%s", hint);
		gets(input);
		if(isExit(input)) {
			printf("bye~\n");
			exit(0);
		}
	}while(!(operationType = getOperationType(input)));
	
	return operationType;
}

char getOperationType(char* str) {
	
	int len = strlen(str);
	char operationType = 0;
	int i;
	for(i = 0; i < len; i++) {
		if(isspace(str[i])) {
			continue;
		}
		
		if(operationType == 0) {
			operationType = 1;
			if(str[i] == '+') {
				operationType = '+';
			}
			else if(str[i] == '-') {
				operationType = '-';
			}
			else if(str[i] == '*') {
				operationType = '*';
			}
			else {
				operationType = 0;
				break;
			}
		}
		else{
			operationType = 0;
			break;
		}
	}
	
	return operationType;
}

polynomial* operation(char operationType, polynomial* p1, polynomial* p2) {
	
	
	return p1;
}

polynomial* addition(polynomial* p1, polynomial* p2) {
	return p1;
}

polynomial* subtraction(polynomial* p1, polynomial* p2) {
	return p1;
}

polynomial* multiplication(polynomial* p1, polynomial* p2) {
	return p1;
}

polynomial* stringToPolynomial(char* polynomialStr) {
	polynomial *top = NULL;
	
	char signOfCoef = 1;
	int coefficient;
	int exponent;
	char charBuff;
	int i;
	for(i = 0; i < len; i++) {
		if(isspace(polynomialStr[i])) {
			continue;
		}
		charBuff = polynomialStr[i];
		
		if(charBuff == '+') {
			signOfCoef = 1;
		}
		else if(charBuff == '-') {
			signOfCoef = -1;
		}
	}
	
	push(top, coefficient, exponent);
	
	return top;
}

void push(polynomial* top, int coefficient, int exponent) {
	polynomial *temp = (polynomial*)malloc(sizeof(polynomial));
	temp->coefficient = coefficient;
	temp->exponent = exponent;
	temp->next = top;
	top = temp;
}

char* polynomialToString(polynomial* p) {
	return "";
}

