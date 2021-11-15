#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	float monny;
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
	int BigStartIndex;
	int BigEndIndex;
} ProcessRecord;

recipes* readRecipes();
OrderContent* readOrder();
void freeOrder(OrderContent**);
void initialPlayer(Player*, int);
void resetPlayer(Player*, int);
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
	
	while(TRUE) {
		OrderContent *tempOrder = Order;
		Order = readOrder();
		orderOrder(Order, numOfOrder);
		float monnyAVG = 0;
		int i, j;

		if(tempOrder) {
			for(i = 0; i < numOfOrder; i++) {
				if(tempOrder[i].everDo == 1 && tempOrder[i].Done == 0 || tempOrder[i].everDo == 0 && tempOrder[i].Done == 1) {
					Order[i].Done = 1;
				}
			}
			freeOrder(&tempOrder);
		}
		else {
			initialPlayer(&p1, Order[0].receiveTime);
			initialPlayer(&p2, Order[0].receiveTime);
			initialProcessRecord(&ProcessRecord);
		}
		
		int maxDeadline = 0;
		for(i = 0; i < numOfOrder; i++) {
			if(Order[i].Done != 1 && Order[i].finishTime > maxDeadline) {
				maxDeadline = Order[i].finishTime;
			}
		}
		
		int t;
		for(t = Order[0].receiveTime; t < maxDeadline; /*pass*/) {
			
			for(i = ProcessRecord.BigStartIndex; i < numOfOrder; i++) {
				if(t >= Order[i].receiveTime && t < Order[i].finishTime){
					ProcessRecord.BigStartIndex = i;
					break;
				}
			}
			if(i == numOfOrder) {
				break;
			}
			for(i = ProcessRecord.BigStartIndex; i < numOfOrder; i++) {
				if(!(t >= Order[i].receiveTime && t < Order[i].finishTime)){
					ProcessRecord.BigEndIndex = i;
					break;
				}
			}
			if(i == numOfOrder) {
				ProcessRecord.BigEndIndex = numOfOrder;
			}
			
			int equalFlag = (p1.time == p2.time);
			Player *ptemp = (p1.time <= p2.time)? &p1 : &p2;
			//printf("p1:%d p2:%d t:%d s:%d e:%d\n", p1.time, p2.time, t, ProcessRecord.BigStartIndex, ProcessRecord.BigEndIndex);
			int NothingToDoFlag = 0;
			int NothingToDo[20];
			int NothingToDoIndex = 0;
			while(TRUE) {
				OrderContent *maxOrder = NULL;
				for(i = ProcessRecord.BigStartIndex; i < ProcessRecord.BigEndIndex; i++) {
					if(Order[i].Done == 0) {
						int flag = 0;
						for(j = 0; j < NothingToDoIndex; j++) {
							if(Order[i].number == NothingToDo[j]) {
								flag = 1;
							}
						}
						if(flag) {
							continue;
						}
						maxOrder = &Order[i];
						break;
					}
				}
				if(maxOrder == NULL) {
					NothingToDoFlag = 1;
					break;
				}
				for(i = i; i < ProcessRecord.BigEndIndex; i++) {
					if(Order[i].Done == 0 && (5 * maxOrder->cookNum + 3 * maxOrder->cutNum + maxOrder->otherTime) < (5 * Order[i].cookNum + 3 * Order[i].cutNum + Order[i].otherTime)) {
						int flag = 0;
						for(j = 0; j < NothingToDoIndex; j++) {
							if(Order[i].number == NothingToDo[j]) {
								flag = 1;
							}
						}
						if(flag) {
							continue;
						}
						maxOrder = &Order[i];
					}
				}
				
				if(ptemp->time >= ProcessRecord.cookTime && maxOrder->cookNum != 0 && ptemp->time + 5 < maxOrder->finishTime) {
					maxOrder->everDo = 1;
					Process *temp = (Process*)malloc(sizeof(Process));
					temp->number = maxOrder->number;
					temp->processTime = ptemp->time;
					ptemp->time += 5;
					if(maxOrder->CanOutTime < ptemp->time) {
						maxOrder->CanOutTime = ptemp->time;
					}
					ProcessRecord.cookTime = ptemp->time;
					temp->type = 's';
					strcpy(temp->vegetableToProcess, maxOrder->cook->cookWhat);
					Vegetable* tempVegetable = maxOrder->cook;
					maxOrder->cook = maxOrder->cook->next;
					free(tempVegetable);
					maxOrder->cookNum--;
					temp->next = ptemp->Head;
					ptemp->Head = temp;
					break;
				}
				if(ptemp->time >= ProcessRecord.cutTime && maxOrder->cutNum != 0 && ptemp->time + 3 < maxOrder->finishTime) {
					maxOrder->everDo = 1;
					Process *temp = (Process*)malloc(sizeof(Process));
					temp->number = maxOrder->number;
					temp->processTime = ptemp->time;
					ptemp->time += 3;
					if(maxOrder->CanOutTime < ptemp->time) {
						maxOrder->CanOutTime = ptemp->time;
					}
					ProcessRecord.cutTime = ptemp->time;
					temp->type = 'c';
					strcpy(temp->vegetableToProcess, maxOrder->cut->cookWhat);
					Vegetable* tempVegetable = maxOrder->cut;
					maxOrder->cut = maxOrder->cut->next;
					free(tempVegetable);
					maxOrder->cutNum--;
					temp->next = ptemp->Head;
					ptemp->Head = temp;
					break;
				}
				if(maxOrder->cookNum == 0 && maxOrder->cutNum == 0 && ptemp->time >= maxOrder->CanOutTime && ptemp->time + maxOrder->otherTime <= maxOrder->finishTime) {
					Process *temp = (Process*)malloc(sizeof(Process));
					temp->number = maxOrder->number;
					temp->processTime = ptemp->time;
					ptemp->time += maxOrder->otherTime;
					temp->type = 'f';
					maxOrder->Done = 1;
					temp->next = ptemp->Head;
					ptemp->Head = temp;
					break;
				}	
				NothingToDo[NothingToDoIndex++] = maxOrder->number;
			}
			
			if(NothingToDoFlag) {
				if(equalFlag) {
					p1.time++;
					p2.time++;
					t++;
				}
				else {
					ptemp->time++;
					t++;
				}
			}
			else {
				t = (p1.time <= p2.time)? p1.time : p2.time;
			}
		}
		
		int doOverFlag = 0;
		for(i = 0; i < numOfOrder; i++) {
			if(Order[i].everDo == 1 && Order[i].Done == 0) {
				doOverFlag = 1;
			}
		}
		if(!doOverFlag) {
			break;
		}
		resetPlayer(&p1, Order[0].receiveTime);
		resetPlayer(&p2, Order[0].receiveTime);
		resetProcessRecord(&ProcessRecord);
	}
	
	Process *tempProcessHead = NULL;

	while(p1.Head) {
		Process *tempProcess = p1.Head;
		p1.Head = p1.Head->next;
		tempProcess->next = tempProcessHead;
		tempProcessHead = tempProcess;
	}
	p1.Head = tempProcessHead;
	
	tempProcessHead = NULL;
	while(p2.Head) {
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
		order[i].monny = 1.0 * (Revenue - Loss) / (5 * order[i].cookNum + 3 * order[i].cutNum + order[i].otherTime);
	}
	
	fclose(fin);
	
	return order;
}

void freeOrder(OrderContent** order) {
	int i;
	for(i = 0; i < numOfOrder; i++) {
		//printf("%d %d %d\n", (*order+i)->number, (*order+i)->receiveTime, (*order+i)->finishTime);
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

void initialPlayer(Player *p, int startTime) {
	p->time = startTime;
	p->Head = NULL;
}

void resetPlayer(Player *p, int startTime) {
	p->time = startTime;
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
			if(orders[j].finishTime < orders[swapIndex].finishTime) {
				swapIndex = j;
			}
		}
		if(i != swapIndex) {
			OrderContent temp = orders[i];
			orders[i] = orders[swapIndex];
			orders[swapIndex] = temp;
		} 
	}
	for(i = 0; i < num-2; i++) {
		swapIndex = i;
		for(j = i; j < num-1; j++) {
			if(orders[j].receiveTime > orders[j+1].receiveTime) {
				OrderContent temp = orders[j];
				orders[j] = orders[j+1];
				orders[j+1] = temp;
			}
		}
	}
}
