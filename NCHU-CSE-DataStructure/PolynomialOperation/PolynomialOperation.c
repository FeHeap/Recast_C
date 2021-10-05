#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct polynomial polynomial;
struct polynomial {
	int coefficient;
	int exponent;
	polynomial *next;
};

polynomial* inputPolynomial(char*);
int isValid(char*);
int isExit(char*);
int getOperationType(char*);
int inputOperationSymbol(char*);
polynomial* operation(int, polynomial*, polynomial*);
polynomial* addition(polynomial*, polynomial*);
polynomial* subtraction(polynomial*, polynomial*);
polynomial* multiplication(polynomial*, polynomial*);
polynomial* stringToPolynomial(char*);
char* polynomialToString(polynomial*);

int main() {
	printf("Input polynomials and operation symbol('*'¡B'-'¡B'+')\n");
	polynomial *p1 = inputPolynomial("Polynomial 1:");
	polynomial *p2 = inputPolynomial("Polynomial 2:");
	int operationSymbol = inputOperationSymbol("Operation Symbol:");
	polynomial* ans = operation(operationSymbol, p1, p2);
	printf("Ans: %s\n", polynomialToString(ans));
	
	system("PAUSE");
	return 0;
}

polynomial* inputPolynomial(char* hint) {
	char input[100];
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
		if(polynomialStr[i] == ' ') {
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
		if(str[i] == ' ' || str[i] == '\t') {
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

int inputOperationSymbol(char* hint) {
	return 0;
}

int getOperationType(char* hint) {
	return 0;
}

polynomial* operation(int operationType, polynomial* p1, polynomial* p2) {
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
	
	return ;
}

char* polynomialToString(polynomial* p) {
	return "";
}

