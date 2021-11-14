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

	initialPlayer(&p1);
	initialPlayer(&p2);
	initialProcessRecord(&ProcessRecord);
	Order = readOrder();
	orderOrder(Order, numOfOrder);
	int i, j;
	int maxDeadline = 0;
	for(i = 0; i < numOfOrder; i++) {
		if(Order[i].finishTime > maxDeadline) {
			maxDeadline = Order[i].finishTime;
		}
	}
	int t;
	for(t = 0; t < maxDeadline; t++) {
		
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
	char charBuff;
	fscanf(fin, "%d", &numOfOrder);
	charBuff = fgetc(fin);
	OrderContent *order = (OrderContent*)malloc(numOfOrder * sizeof(OrderContent));
	int i, j, k;
	for(i = 0; i < numOfOrder; i++) {
		order[i].DoOrNot = 0;
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
		printf("%d\n", (*order+i)->number);
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

void orderOrder(OrderContent *orders, int num) {
	int i, j, swapIndex;
	for(i = 0; i < num-1; i++) {
		swapIndex = i;
		for(j = i+1; j < num; j++) {
			if(orders[j].receiveTime < orders[swapIndex].receiveTime) {
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
