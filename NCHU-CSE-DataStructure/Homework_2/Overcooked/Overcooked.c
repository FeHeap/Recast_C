#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECIPES_FIN "test_case_2-1/recipes.txt" 
#define ORDERS_FIN "test_case_2-1/orders.txt" 
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
	int DoOrNot;
	int number;
	int cookNum;
	Vegetable *cook;
	int cutNum;
	Vegetable *cut;
	int otherTime;
	int monny;
	int receiveTime;
	int finishTime;
	struct orderContent *next;
} OrderContent;
OrderContent *Order;

typedef struct process {
	int processTime;
	int number;
	char type;
	char vegetableToProcess[15];
	struct process *next;
} Process;

typedef struct player {
	int time;
	Process *Head;
} Player;
Player p1, p2;

struct processRecord {
	int cookTime;
	int cutTime;
	int num;
	int notToDo[200];
} ProcessRecord;

CookingTime* readRecipes();
OrderContent* readOrder();
void freeOrder(OrderContent**);
void initialPlayer(Player*);
void resetPlayer(Player*);
void initialProcessRecord(struct processRecord*);
void resetProcessRecord(struct processRecord*);
int halfOrderDetect(OrderContent*);

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

	initialPlayer(&p1);
	initialPlayer(&p2);
	initialProcessRecord(&ProcessRecord);
	
	int i, j;
	while(TRUE) {
		Order = readOrder();
		OrderContent *orderTemp;
		for(i = 0; i < ProcessRecord.num; i++) {
			orderTemp = Order;
			if(ProcessRecord.notToDo[i] == Order->number) {
				Order= Order->next;
				Vegetable *tempVegetable = orderTemp->cook;
				while(tempVegetable != NULL) {
					Vegetable *subTempVegetable = tempVegetable;
					tempVegetable = tempVegetable->next;
					free(subTempVegetable);
				}
				tempVegetable = orderTemp->cut;
				while(tempVegetable != NULL) {
					Vegetable *subTempVegetable = tempVegetable;
					tempVegetable = tempVegetable->next;
					free(subTempVegetable);
				}
				free(orderTemp);
			}
			else {
				while(ProcessRecord.notToDo[i] != orderTemp->next->number) {
					orderTemp = orderTemp->next;
				}
				OrderContent *orderTempNext = orderTemp->next;
				orderTemp->next = orderTemp->next->next;
				Vegetable *tempVegetable = orderTempNext->cook;
				while(tempVegetable != NULL) {
					Vegetable *subTempVegetable = tempVegetable;
					tempVegetable = tempVegetable->next;
					free(subTempVegetable);
				}
				tempVegetable = orderTempNext->cut;
				while(tempVegetable != NULL) {
					Vegetable *subTempVegetable = tempVegetable;
					tempVegetable = tempVegetable->next;
					free(subTempVegetable);
				}
				free(orderTempNext);
			}	
		}
		
		
		if(halfOrderDetect(Order)) {
			orderTemp = Order;
			while(orderTemp) {
				if(orderTemp->DoOrNot == 1) {
					ProcessRecord.notToDo[ProcessRecord.num++] = orderTemp->number;
				}
				orderTemp = orderTemp->next;	
			}
			resetPlayer(&p1);
			resetPlayer(&p2);
			resetProcessRecord(&ProcessRecord);
			freeOrder(&Order);
		}
		else {
			break;
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

CookingTime* readRecipes() {
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
	OrderContent *order = NULL;
	char charBuff;
	int numOfOrder;
	fscanf(fin, "%d", &numOfOrder);
	charBuff = fgetc(fin);
	int i, j, k;
	for(i = 0; i < numOfOrder; i++) {
		OrderContent *tempOrder = (OrderContent*)malloc(sizeof(OrderContent));
		tempOrder->DoOrNot = 0;
		fscanf(fin, "%d", &tempOrder->number);
		charBuff = fgetc(fin);
		tempOrder->cook = NULL;
		tempOrder->cut = NULL;
		char dishBuff[35];
		fscanf(fin, "%[^ ]", dishBuff);
		for(j = 0; j < Recipes->numOfKind; j++) {
			if(!strcmp(Recipes->book[j].dish, dishBuff)) {
				tempOrder->cookNum = Recipes->book[j].cook;
				for(k = 0; k < tempOrder->cookNum; k++) {
					Vegetable *temp = (Vegetable*)malloc(sizeof(Vegetable));
					strcpy(temp->cookWhat, Recipes->book[j].cookWhat[k]);
					temp->next = tempOrder->cook;
					tempOrder->cook = temp;
				}
				tempOrder->cutNum = Recipes->book[j].cut;
				for(k = 0; k < tempOrder->cutNum; k++) {
					Vegetable *temp = (Vegetable*)malloc(sizeof(Vegetable));
					strcpy(temp->cookWhat, Recipes->book[j].cutWhat[k]);
					temp->next = tempOrder->cut;
					tempOrder->cut = temp;
				}
				tempOrder->otherTime = Recipes->book[j].other;
				break;
			}
		}
		charBuff = fgetc(fin);

		fscanf(fin, "%d", &tempOrder->receiveTime);
		charBuff = fgetc(fin);
		fscanf(fin, "%d", &tempOrder->finishTime);
		charBuff = fgetc(fin);
		int Revenue, Loss;
		fscanf(fin, "%d", &Revenue);
		charBuff = fgetc(fin);
		fscanf(fin, "%d", &Loss);
		charBuff = fgetc(fin);
		tempOrder->monny = Revenue - Loss;
		
		tempOrder->next = order;
		order = tempOrder;
	}
	
	fclose(fin);
	
	return order;
}

void freeOrder(OrderContent** order) {
	while((*order) != NULL) {
		OrderContent *tempOrder = (*order);
		(*order) = (*order)->next;
		Vegetable *tempVegetable = tempOrder->cook;
		while(tempVegetable != NULL) {
			Vegetable *subTempVegetable = tempVegetable;
			tempVegetable = tempVegetable->next;
			free(subTempVegetable);
		}
		tempVegetable = tempOrder->cut;
		while(tempVegetable != NULL) {
			Vegetable *subTempVegetable = tempVegetable;
			tempVegetable = tempVegetable->next;
			free(subTempVegetable);
		}
		free(tempOrder);
	}
}

void initialPlayer(Player *p) {
	p->time = 0;
	p->Head = NULL;
}

void resetPlayer(Player *p) {
	p->time = 0;
	while(p->Head) {
		Process *temp = p->Head;
		p->Head = p->Head->next;
		free(temp);
	}
}

void initialProcessRecord(struct processRecord* ProcessRecord) {
	ProcessRecord->cookTime = 0;
	ProcessRecord->cutTime = 0;
	ProcessRecord->num = 0;
}

void resetProcessRecord(struct processRecord* ProcessRecord) {
	ProcessRecord->cookTime = 0;
	ProcessRecord->cutTime = 0;
}

int halfOrderDetect(OrderContent *order) {
	int flag = 0;
	while(order) {
		if(order->DoOrNot == 1) {
			flag = 1;
			break;
		}
		order = order->next;	
	}
	return flag;
}
