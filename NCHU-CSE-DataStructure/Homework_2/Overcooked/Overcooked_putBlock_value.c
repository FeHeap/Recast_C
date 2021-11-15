#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define RECIPES_FIN "test_case_2-3/recipes.txt" 
#define ORDERS_FIN "test_case_2-3/orders.txt" 
#define PLAYER_FOUT "players.txt"
#define TRUE 1

FILE *fin, *fout;

typedef struct cookingTime {
	char dish[35];
	int cook;
	char cookWhat[5][15];
	int cut;
	char cutWhat[5][15];
	int other;
} CookingTime;

typedef struct recipes {
	int numOfKind;
	CookingTime *book;
} recipes;
recipes *Recipes;

typedef struct vegetable {
	char cookWhat[15];
	struct vegetable *next;
} Vegetable;

typedef struct orderContent {
	int Done;
	int CanOutTime;
	int everDo;
	
	int number;
	int cookNum;
	Vegetable *cook;
	int cutNum;
	Vegetable *cut;
	int otherTime;
	int monny;
	int receiveTime;
	int finishTime;
} OrderContent;
OrderContent *Order;
int numOfOrder;

typedef struct process {
	int processTime;
	int number;
	char type;
	char vegetableToProcess[15];
	struct process *next;
} Process;

typedef struct player {
	Process *Head;
} Player;
Player p1, p2;

struct processRecord {
	int cookTime;
	int cutTime;
	int BigStartIndex;
	int BigEndIndex;
} ProcessRecord;

recipes* readRecipes();
OrderContent* readOrder();
void freeOrder(OrderContent**);
void initialPlayer(Player*);
void resetPlayer(Player*);
void initialProcessRecord(struct processRecord*);
void resetProcessRecord(struct processRecord*);
void orderOrder(OrderContent*, int);


int main() {
	// detect whether PLAYER_FOUT has already existed
	if(!((fout = fopen(PLAYER_FOUT, "r")) == NULL)) {
		fclose(fin);
		printf("File %s has already existed! Overwrite?[Y/n] ", PLAYER_FOUT);
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
	
	Recipes = readRecipes();
	Order = NULL;
	
	
	Order = readOrder();
	orderOrder(Order, numOfOrder);
	
	initialPlayer(&p1);
	initialPlayer(&p2);
	
	int i, j, k;
	int maxDeadline = 0;
	for(i = 0; i < numOfOrder; i++) {
		if(Order[i].Done != 1 && Order[i].finishTime > maxDeadline) {
			maxDeadline = Order[i].finishTime;
		}
	}
	int *cookBlock = (int*)malloc(maxDeadline * sizeof(int));
	int *cutBlock = (int*)malloc(maxDeadline * sizeof(int));
	int *p1Block = (int*)malloc(maxDeadline * sizeof(int));
	int *p2Block = (int*)malloc(maxDeadline * sizeof(int));
	for(i = 0; i < maxDeadline; i++) {
		cookBlock[i] = 0;
		cutBlock[i] = 0;
		p1Block[i] = 0;
		p2Block[i] = 0;
	}
	
	
	for(i = 0; i < numOfOrder; i++) {
		
		int fStart = 0;
		int cookCondition = 0;
		int p1js, p2js;
		if(Order[i].cookNum != 0) {
			for(p1js = Order[i].receiveTime; p1js < Order[i].finishTime - Order[i].otherTime - 5 * Order[i].cookNum; p1js++) {
				int flag = 1;
				for(k = 0; k < 5 * Order[i].cookNum; k++) {
					if(p1Block[p1js + k] != 0 || cookBlock[p1js + k] != 0) {
						flag = 0;
						break;
					}
				}
				if(flag) {
					break;
				}
			}
			
			for(p2js = Order[i].receiveTime; p2js < Order[i].finishTime - Order[i].otherTime - 5 * Order[i].cookNum; p2js++) {
				int flag = 1;
				for(k = 0; k < 5 * Order[i].cookNum; k++) {
					if(p2Block[p2js + k] != 0 || cookBlock[p2js + k] != 0) {
						flag = 0;
						break;
					}
				}
				if(flag) {
					break;
				}
			}
			
			if(p1js >= Order[i].finishTime - Order[i].otherTime - 5 * Order[i].cookNum && p2js >= Order[i].finishTime - Order[i].otherTime - 5 * Order[i].cookNum) {
				continue;
			}
			else if(p1js < Order[i].finishTime - Order[i].otherTime - 5 * Order[i].cookNum && p2js >= Order[i].finishTime - Order[i].otherTime - 5 * Order[i].cookNum) {
				cookCondition = 1;
				for(k = 0; k < 5 * Order[i].cookNum; k++) {
					p1Block[p1js + k] = 1;
					cookBlock[p1js + k] = 1;
				}
				if(p1js + 5 * Order[i].cookNum > fStart) {
					fStart = p1js + 5 * Order[i].cookNum;
				}
			}
			else if(p1js >= Order[i].finishTime - Order[i].otherTime - 5 * Order[i].cookNum && p2js < Order[i].finishTime - Order[i].otherTime - 5 * Order[i].cookNum) {
				cookCondition = 2;
				for(k = 0; k < 5 * Order[i].cookNum; k++) {
					p2Block[p2js + k] = 1;
					cookBlock[p2js + k] = 1;
				}
				if(p2js + 5 * Order[i].cookNum > fStart) {
					fStart = p2js + 5 * Order[i].cookNum;
				}
			}
			else {
				if(p1js <= p2js) {
					cookCondition = 1;
					for(k = 0; k < 5 * Order[i].cookNum; k++) {
						p1Block[p1js + k] = 1;
						cookBlock[p1js + k] = 1;
					}
					if(p1js + 5 * Order[i].cookNum > fStart) {
						fStart = p1js + 5 * Order[i].cookNum;
					}
				}
				else {
					cookCondition = 2;
					for(k = 0; k < 5 * Order[i].cookNum; k++) {
						p2Block[p2js + k] = 1;
						cookBlock[p2js + k] = 1;
					}
					if(p2js + 5 * Order[i].cookNum > fStart) {
						fStart = p2js + 5 * Order[i].cookNum;
					}
				}
			}
		}
		
		int cutCondition = 0;
		int p1jc, p2jc;
		if(Order[i].cutNum != 0) {
			for(p1jc = Order[i].receiveTime; p1jc < Order[i].finishTime - Order[i].otherTime - 3 * Order[i].cutNum; p1jc++) {
				int flag = 1;
				for(k = 0; k < 3 * Order[i].cutNum; k++) {
					if(p1Block[p1jc + k] != 0 || cutBlock[p1jc + k] != 0) {
						flag = 0;
						break;
					}
				}
				if(flag) {
					break;
				}
			}
			
			for(p2jc = Order[i].receiveTime; p2jc < Order[i].finishTime - Order[i].otherTime - 3 * Order[i].cutNum; p2jc++) {
				int flag = 1;
				for(k = 0; k < 3 * Order[i].cutNum; k++) {
					if(p2Block[p2jc + k] != 0 || cutBlock[p2jc + k] != 0) {
						flag = 0;
						break;
					}
				}
				if(flag) {
					break;
				}
			}
			//printf("%d\n",i);
			if(p1jc >= Order[i].finishTime - Order[i].otherTime - 3 * Order[i].cutNum && p2jc >= Order[i].finishTime - Order[i].otherTime - 3 * Order[i].cutNum) {
				if(cookCondition == 1) {
					for(k = 0; k < 5 * Order[i].cookNum; k++) {
						p1Block[p1js + k] = 0;
						cookBlock[p1js + k] = 0;
					}
				}
				else if(cookCondition == 2) {
					for(k = 0; k < 5 * Order[i].cookNum; k++) {
						p2Block[p2js + k] = 0;
						cookBlock[p2js + k] = 0;
					}
				}
				continue;
			}
			else if(p1jc < Order[i].finishTime - Order[i].otherTime - 3 * Order[i].cutNum && p2jc >= Order[i].finishTime - Order[i].otherTime - 3 * Order[i].cutNum) {
				cutCondition = 1;
				for(k = 0; k < 3 * Order[i].cutNum; k++) {
					p1Block[p1jc + k] = 1;
					cutBlock[p1jc + k] = 1;
				}
				if(p1jc + 3 * Order[i].cutNum > fStart) {
					fStart = p1jc + 3 * Order[i].cutNum;
				}
			}
			else if(p1jc >= Order[i].finishTime - Order[i].otherTime - 3 * Order[i].cutNum && p2jc < Order[i].finishTime - Order[i].otherTime - 3 * Order[i].cutNum) {
				cutCondition = 2;
				for(k = 0; k < 3 * Order[i].cutNum; k++) {
					p2Block[p2jc + k] = 1;
					cutBlock[p2jc + k] = 1;
				}
				if(p2jc + 3 * Order[i].cutNum > fStart) {
					fStart = p2jc + 3 * Order[i].cutNum;
				}
			}
			else {
				if(p1jc <= p2jc) {
					cutCondition = 1;
					for(k = 0; k < 3 * Order[i].cutNum; k++) {
						p1Block[p1jc + k] = 1;
						cutBlock[p1jc + k] = 1;
					}
					if(p1jc + 3 * Order[i].cutNum > fStart) {
						fStart = p1jc + 3 * Order[i].cutNum;
					}
				}
				else {
					cutCondition = 2;
					for(k = 0; k < 3 * Order[i].cutNum; k++) {
						p2Block[p2jc + k] = 1;
						cutBlock[p2jc + k] = 1;
					}
					if(p2jc + 3 * Order[i].cutNum > fStart) {
						fStart = p2jc + 3 * Order[i].cutNum;
					}
				}
			}
		}
		
		int fCondition;
		int p1jf, p2jf;
		for(p1jf = fStart; p1jf < Order[i].finishTime - Order[i].otherTime; p1jf++) {
			int flag = 1;
			for(k = 0; k < Order[i].otherTime; k++) {
				if(p1Block[p1jf + k] != 0) {
					flag = 0;
					break;
				}
			}
			if(flag) {
				break;
			}
		}
		
		for(p2jf = fStart; p2jf < Order[i].finishTime - Order[i].otherTime; p2jf++) {
			int flag = 1;
			for(k = 0; k < Order[i].otherTime; k++) {
				if(p2Block[p2jf + k] != 0) {
					flag = 0;
					break;
				}
			}
			if(flag) {
				break;
			}
		}
		
		if(p1jf >= Order[i].finishTime - Order[i].otherTime && p2jf >= Order[i].finishTime - Order[i].otherTime) {
			if(cookCondition == 1) {
				for(k = 0; k < 5 * Order[i].cookNum; k++) {
					p1Block[p1js + k] = 0;
					cookBlock[p1js + k] = 0;
				}
			}
			else if(cookCondition == 2) {
				for(k = 0; k < 5 * Order[i].cookNum; k++) {
					p2Block[p2js + k] = 0;
					cookBlock[p2js + k] = 0;
				}
			}
			
			if(cutCondition == 1) {
				for(k = 0; k < 3 * Order[i].cutNum; k++) {
					p1Block[p1jc + k] = 0;
					cutBlock[p1jc + k] = 0;
				}
			}
			else if(cutCondition == 2) {
				for(k = 0; k < 3 * Order[i].cutNum; k++) {
					p2Block[p2jc + k] = 0;
					cutBlock[p2jc + k] = 0;
				}
			}
			
			continue;
		}
		else if(p1jf < Order[i].finishTime - Order[i].otherTime && p2jf >= Order[i].finishTime - Order[i].otherTime) {
			fCondition = 1;
			for(k = 0; k < Order[i].otherTime; k++) {
				p1Block[p1jf + k] = 1;
			}
		}
		else if(p1jf >= Order[i].finishTime - Order[i].otherTime && p2jf < Order[i].finishTime - Order[i].otherTime) {
			fCondition = 2;
			for(k = 0; k < Order[i].otherTime; k++) {
				p2Block[p2jf + k] = 1;
			}
		}
		else {
			if(p1jf <= p2jf) {
				fCondition = 1;
				for(k = 0; k < Order[i].otherTime; k++) {
					p1Block[p1jf + k] = 1;
				}
			}
			else {
				fCondition = 2;
				for(k = 0; k < Order[i].otherTime; k++) {
					p2Block[p2jf + k] = 1;
				}
			}
		}
		
		if(cookCondition == 1) {
			Vegetable *temp = Order[i].cook;
			while(temp) {
				Process *tempProcess = (Process*)malloc(sizeof(Process));
				tempProcess->number = Order[i].number;
				tempProcess->processTime = p1js;
				p1js += 5;
				tempProcess->type = 's';
				strcpy(tempProcess->vegetableToProcess, temp->cookWhat);
				
				if(p1.Head == NULL) {
					tempProcess->next = p1.Head;
					p1.Head = tempProcess;
				}
				else if(tempProcess->processTime > p1.Head->processTime) {
					tempProcess->next = p1.Head;
					p1.Head = tempProcess;
				}
				else {
					Process *subTempProcess = p1.Head;
					while(subTempProcess->next != NULL) {
						if(subTempProcess->processTime > tempProcess->processTime && tempProcess->processTime > subTempProcess->next->processTime) {
							break;
						}
						subTempProcess = subTempProcess->next;
					}
					tempProcess->next = subTempProcess->next;
					subTempProcess->next = tempProcess;
				}
				temp = temp->next;
			}
		}
		
		else if(cookCondition == 2) {
			Vegetable *temp = Order[i].cook;
			while(temp) {
				Process *tempProcess = (Process*)malloc(sizeof(Process));
				tempProcess->number = Order[i].number;
				tempProcess->processTime = p2js;
				p2js += 5;
				tempProcess->type = 's';
				strcpy(tempProcess->vegetableToProcess, temp->cookWhat);
				
				if(p2.Head == NULL) {
					tempProcess->next = p2.Head;
					p2.Head = tempProcess;
				}
				else if(tempProcess->processTime > p2.Head->processTime) {
					tempProcess->next = p2.Head;
					p2.Head = tempProcess;
				}
				else {
					Process *subTempProcess = p2.Head;
					while(subTempProcess->next != NULL) {
						if(subTempProcess->processTime > tempProcess->processTime && tempProcess->processTime > subTempProcess->next->processTime) {
							break;
						}
						subTempProcess = subTempProcess->next;
					}
					tempProcess->next = subTempProcess->next;
					subTempProcess->next = tempProcess;
				}
				temp = temp->next;
			}
		}
		
		
		if(cutCondition == 1) {
			Vegetable *temp = Order[i].cut;
			while(temp) {
				Process *tempProcess = (Process*)malloc(sizeof(Process));
				tempProcess->number = Order[i].number;
				tempProcess->processTime = p1jc;
				p1jc += 3;
				tempProcess->type = 'c';
				strcpy(tempProcess->vegetableToProcess, temp->cookWhat);
				
				if(p1.Head == NULL) {
					tempProcess->next = p1.Head;
					p1.Head = tempProcess;
				}
				else if(tempProcess->processTime > p1.Head->processTime) {
					tempProcess->next = p1.Head;
					p1.Head = tempProcess;
				}
				else {
					Process *subTempProcess = p1.Head;
					while(subTempProcess->next != NULL) {
						if(subTempProcess->processTime > tempProcess->processTime && tempProcess->processTime > subTempProcess->next->processTime) {
							break;
						}
						subTempProcess = subTempProcess->next;
					}
					tempProcess->next = subTempProcess->next;
					subTempProcess->next = tempProcess;
				}
				temp = temp->next;
			}
		}
		else if(cutCondition == 2) {
			Vegetable *temp = Order[i].cut;
			while(temp) {
				Process *tempProcess = (Process*)malloc(sizeof(Process));
				tempProcess->number = Order[i].number;
				tempProcess->processTime = p2jc;
				//printf("%d\n", p2jc);
				p2jc += 3;
				tempProcess->type = 'c';
				strcpy(tempProcess->vegetableToProcess, temp->cookWhat);
				
				if(p2.Head == NULL) {
					tempProcess->next = p2.Head;
					p2.Head = tempProcess;
				}
				else if(tempProcess->processTime > p2.Head->processTime) {
					tempProcess->next = p2.Head;
					p2.Head = tempProcess;
				}
				else {
					Process *subTempProcess = p2.Head;
					while(subTempProcess->next != NULL) {
						if(subTempProcess->processTime > tempProcess->processTime && tempProcess->processTime > subTempProcess->next->processTime) {
							break;
						}
						subTempProcess = subTempProcess->next;
					}
					tempProcess->next = subTempProcess->next;
					subTempProcess->next = tempProcess;
				}
				temp = temp->next;
			}
		}
		
		if(fCondition == 1) {
			Process *tempProcess = (Process*)malloc(sizeof(Process));
			tempProcess->number = Order[i].number;
			tempProcess->processTime = p1jf;
			tempProcess->type = 'f';
			
			
			if(p1.Head == NULL) {
				tempProcess->next = p1.Head;
				p1.Head = tempProcess;
			}
			else if(tempProcess->processTime > p1.Head->processTime) {
				tempProcess->next = p1.Head;
				p1.Head = tempProcess;
			}
			else {
				Process *subTempProcess = p1.Head;
				while(subTempProcess->next != NULL) {
					if(subTempProcess->processTime > tempProcess->processTime && tempProcess->processTime > subTempProcess->next->processTime) {
						break;
					}
					subTempProcess = subTempProcess->next;
				}
				tempProcess->next = subTempProcess->next;
				subTempProcess->next = tempProcess;
			}
		}
		else if(fCondition == 2) {
			Process *tempProcess = (Process*)malloc(sizeof(Process));
			tempProcess->number = Order[i].number;
			tempProcess->processTime = p2jf;
			tempProcess->type = 'f';
			
			if(p2.Head == NULL) {
				tempProcess->next = p2.Head;
				p2.Head = tempProcess;
			}
			else if(tempProcess->processTime > p2.Head->processTime) {
				tempProcess->next = p2.Head;
				p2.Head = tempProcess;
			}
			else {
				Process *subTempProcess = p2.Head;
				while(subTempProcess->next != NULL) {
					if(subTempProcess->processTime > tempProcess->processTime && tempProcess->processTime > subTempProcess->next->processTime) {
						break;
					}
					subTempProcess = subTempProcess->next;
				}
				tempProcess->next = subTempProcess->next;
				subTempProcess->next = tempProcess;
			}
		}
	}
	
	
	Process *tempProcessHead = NULL;

	while(p1.Head) {
		printf("p1 %d %c %d\n", p1.Head->processTime, p1.Head->type, p1.Head->number);
		Process *tempProcess = p1.Head;
		p1.Head = p1.Head->next;
		tempProcess->next = tempProcessHead;
		tempProcessHead = tempProcess;
	}
	p1.Head = tempProcessHead;
	
	tempProcessHead = NULL;
	while(p2.Head) {
		printf("p2 %d %c %d\n", p2.Head->processTime, p2.Head->type, p2.Head->number);
		Process *tempProcess = p2.Head;
		p2.Head = p2.Head->next;
		tempProcess->next = tempProcessHead;
		tempProcessHead = tempProcess;
	}
	p2.Head = tempProcessHead;
	while(p1.Head || p2.Head) {
		if(p1.Head && p2.Head) {
			if(p1.Head->processTime <= p2.Head->processTime) {
				if(p1.Head->type != 'f')
					printf("1 %d %d %c %s\n", p1.Head->processTime, p1.Head->number, p1.Head->type, p1.Head->vegetableToProcess);
				else
					printf("1 %d %d %c\n", p1.Head->processTime, p1.Head->number, p1.Head->type);
				p1.Head = p1.Head->next;
			}
			else {
				if(p2.Head->type != 'f')
					printf("2 %d %d %c %s\n", p2.Head->processTime, p2.Head->number, p2.Head->type, p2.Head->vegetableToProcess);
				else
					printf("2 %d %d %c\n", p2.Head->processTime, p2.Head->number, p2.Head->type);
				p2.Head = p2.Head->next;	
			}
		}
		else if(p1.Head) {
			if(p1.Head->type != 'f')
				printf("1 %d %d %c %s\n", p1.Head->processTime, p1.Head->number, p1.Head->type, p1.Head->vegetableToProcess);
			else
				printf("1 %d %d %c\n", p1.Head->processTime, p1.Head->number, p1.Head->type);
			p1.Head = p1.Head->next;
		}
		else {
			if(p2.Head->type != 'f')
				printf("2 %d %d %c %s\n", p2.Head->processTime, p2.Head->number, p2.Head->type, p2.Head->vegetableToProcess);
			else
				printf("2 %d %d %c\n", p2.Head->processTime, p2.Head->number, p2.Head->type);
			p2.Head = p2.Head->next;
		}
	}

	
	// output file (players) opened with mode "w"
	fout = fopen(PLAYER_FOUT, "w");
	
	
	fclose(fout);
	
	/* free Recipes */
	free(Recipes->book);
	free(Recipes);
	
	/* free Order */
	freeOrder(&Order);
	
	system("PAUSE");
	return 0;
}

recipes* readRecipes() {
	// input file (recipes) opened with mode "r"
	if((fin = fopen(RECIPES_FIN, "r")) == NULL) {
		printf("Fail to open file %s!", RECIPES_FIN);
		exit(EXIT_FAILURE);
	}
	recipes *tempRecipes = (recipes*)malloc(sizeof(recipes));
	char charBuff;
	fscanf(fin, "%d", &tempRecipes->numOfKind);
	charBuff = fgetc(fin); // buff '\n'
	tempRecipes->book = (CookingTime*)malloc(tempRecipes->numOfKind * sizeof(CookingTime));
	int i, j;
	for(i = 0; i < tempRecipes->numOfKind; i++) {
		/* dish name */
		fscanf(fin, "%[^ ]", tempRecipes->book[i].dish);
		charBuff = fgetc(fin);
		for(j = 0, charBuff = '\0'; charBuff != ' '; j++) {
			fscanf(fin, "%[^ ,]", tempRecipes->book[i].cookWhat[j]);
			charBuff = fgetc(fin);
			if(!strcmp(tempRecipes->book[i].cookWhat[j], "x")) {
				break;
			}
		}
		tempRecipes->book[i].cook = j;
		for(j = 0, charBuff = '\0'; charBuff != ' '; j++) {
			fscanf(fin, "%[^ ,]", tempRecipes->book[i].cutWhat[j]);
			charBuff = fgetc(fin);
			if(!strcmp(tempRecipes->book[i].cutWhat[j], "x")) {
				break;
			}
		}
		tempRecipes->book[i].cut = j;
		char otherBuff[15];
		for(j = 0, charBuff = '\0'; charBuff != '\n' && charBuff != EOF; j++) {
			fscanf(fin, "%[^\n,]", otherBuff);
			charBuff = fgetc(fin);
			if(!strcmp(otherBuff, "x")) {
				break;
			}
		}
		tempRecipes->book[i].other = 1 + j;
	}
	
	return tempRecipes;
}

OrderContent* readOrder() {
	// input file (orders) opened with mode "r"
	if((fin = fopen(ORDERS_FIN, "r")) == NULL) {
		printf("Fail to open file %s!", ORDERS_FIN);
		exit(EXIT_FAILURE);
	}
	char charBuff;
	fscanf(fin, "%d", &numOfOrder);
	charBuff = fgetc(fin);
	OrderContent *order = (OrderContent*)malloc(numOfOrder * sizeof(OrderContent));
	int i, j, k;
	for(i = 0; i < numOfOrder; i++) {
		order[i].Done = 0;
		order[i].everDo = 0;
		fscanf(fin, "%d", &order[i].number);
		charBuff = fgetc(fin);
		order[i].cook = NULL;
		order[i].cut = NULL;
		char dishBuff[35];
		fscanf(fin, "%[^ ]", dishBuff);
		for(j = 0; j < Recipes->numOfKind; j++) {
			if(!strcmp(Recipes->book[j].dish, dishBuff)) {
				order[i].cookNum = Recipes->book[j].cook;
				for(k = 0; k < order[i].cookNum; k++) {
					Vegetable *temp = (Vegetable*)malloc(sizeof(Vegetable));
					strcpy(temp->cookWhat, Recipes->book[j].cookWhat[k]);
					temp->next = order[i].cook;
					order[i].cook = temp;
				}
				order[i].cutNum = Recipes->book[j].cut;
				for(k = 0; k < order[i].cutNum; k++) {
					Vegetable *temp = (Vegetable*)malloc(sizeof(Vegetable));
					strcpy(temp->cookWhat, Recipes->book[j].cutWhat[k]);
					temp->next = order[i].cut;
					order[i].cut = temp;
				}
				order[i].otherTime = Recipes->book[j].other;
				break;
			}
		}
		charBuff = fgetc(fin);

		fscanf(fin, "%d", &order[i].receiveTime);
		order[i].CanOutTime = order[i].receiveTime;
		charBuff = fgetc(fin);
		fscanf(fin, "%d", &order[i].finishTime);
		charBuff = fgetc(fin);
		int Revenue, Loss;
		fscanf(fin, "%d", &Revenue);
		charBuff = fgetc(fin);
		fscanf(fin, "%d", &Loss);
		charBuff = fgetc(fin);
		order[i].monny = Revenue - Loss;
	}
	
	fclose(fin);
	
	return order;
}

void freeOrder(OrderContent** order) {
	int i;
	for(i = 0; i < numOfOrder; i++) {
		//printf("%d %d %d %d\n", (*order+i)->number, (*order+i)->receiveTime, (*order+i)->finishTime, (*order+i)->monny);
		Vegetable *tempVegetable = (*order+i)->cook;
		while(tempVegetable != NULL) {
			Vegetable *subTempVegetable = tempVegetable;
			tempVegetable = tempVegetable->next;
			free(subTempVegetable);
		}
		tempVegetable = (*order+i)->cut;
		while(tempVegetable != NULL) {
			Vegetable *subTempVegetable = tempVegetable;
			tempVegetable = tempVegetable->next;
			free(subTempVegetable);
		}
	}
	free(*order);
	*order = NULL;
}

void initialPlayer(Player *p) {
	p->Head = NULL;
}

void resetPlayer(Player *p) {
	while(p->Head) {
		Process *temp = p->Head;
		p->Head = p->Head->next;
		free(temp);
	}
}

void initialProcessRecord(struct processRecord* ProcessRecord) {
	ProcessRecord->cookTime = 0;
	ProcessRecord->cutTime = 0;
	ProcessRecord->BigStartIndex = 0;
	ProcessRecord->BigEndIndex = 1;
}

void resetProcessRecord(struct processRecord* ProcessRecord) {
	ProcessRecord->cookTime = 0;
	ProcessRecord->cutTime = 0;
	ProcessRecord->BigStartIndex = 0;
	ProcessRecord->BigEndIndex = 1;
}

void orderOrder(OrderContent *orders, int num) {
	int i, j, swapIndex;
	for(i = 0; i < num-1; i++) {
		swapIndex = i;
		for(j = i+1; j < num; j++) {
			if(orders[j].monny > orders[swapIndex].monny) {
				swapIndex = j;
			}
		}
		if(i != swapIndex) {
			OrderContent temp = orders[i];
			orders[i] = orders[swapIndex];
			orders[swapIndex] = temp;
		} 
	}
}
