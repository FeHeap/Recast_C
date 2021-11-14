#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define RECIPES_FIN "test_case_2-3/recipes.txt" 
#define ORDERS_FIN "test_case_2-3/orders.txt" 
#define PLAYER_FOUT "players.txt"

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

CookingTime* readRecipes();
OrderContent* readOrder();

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
//	int i, j;
//	printf("%d\n", Recipes->numOfKind);
//	for(i=0; i < Recipes->numOfKind; i++) {
//		printf("%s\n", Recipes->book[i].dish);
//		for(j = 0; j < Recipes->book[i].cook; j++)
//			printf("%s ", Recipes->book[i].cookWhat[j]);
//		printf("\n");
//		for(j = 0; j < Recipes->book[i].cut; j++)
//			printf("%s ", Recipes->book[i].cutWhat[j]);
//		printf("\n");
//		printf("%d\n", Recipes->book[i].other);
//	}
//	printf("=========================\n");
	Order = readOrder();
//	OrderContent *pOrther = Order;
//	while(pOrther != NULL) {
//		OrderContent *tempOrder = pOrther;
//		pOrther = pOrther->next;
//		Vegetable *tempVegetable = tempOrder->cook;
//		printf("%d ", tempOrder->number);
//		printf("%d ", tempOrder->receiveTime);
//		printf("%d ", tempOrder->finishTime);
//		printf("%d ", tempOrder->monny);
//		printf("\n");
//		while(tempVegetable != NULL) {
//			Vegetable *subTempVegetable = tempVegetable;
//			tempVegetable = tempVegetable->next;
//			printf("%s ", subTempVegetable->cookWhat);
//		}
//		printf("\n");
//		tempVegetable = tempOrder->cut;
//		while(tempVegetable != NULL) {
//			Vegetable *subTempVegetable = tempVegetable;
//			tempVegetable = tempVegetable->next;
//			printf("%s ", subTempVegetable->cookWhat);
//		}
//		printf("\n");
//		printf("%d\n", tempOrder->otherTime);
//	}
	
	
	// output file (players) opened with mode "w"
	fout = fopen(PLAYER_FOUT, "w");
	
	
	fclose(fout);
	
	/* free Recipes */
	free(Recipes->book);
	free(Recipes);
	
	/* free Order */
	while(Order != NULL) {
		OrderContent *tempOrder = Order;
		Order = Order->next;
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
 

