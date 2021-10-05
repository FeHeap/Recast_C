#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define LIST_SIZE 10

typedef struct _person{
	char* firstName;
	char* lastName;
	char* title;
	unsigned int age;
}Person;


typedef struct _alternatePerson{
	char* firstName;
	char* lastName;
	char* title;
	unsigned int age;
}AlternatePerson;


void initPerson(Person *person){
	person->firstName = NULL;
	person->lastName = NULL;
	person->title = NULL;
	person->age = 0;
}

void cleanPerson(Person *person){
	if(person->firstName != NULL){ free(person->firstName); }
	if(person->lastName != NULL){ free(person->lastName); }
	if(person->title != NULL){ free(person->title); }
}

void initializePerson(Person *person, const char* fn, const char *ln, const char *title, unsigned int age){
	person->firstName = (char*)malloc(strlen(fn) + 1);
	strcpy(person->firstName, fn);
	person->lastName = (char*)malloc(strlen(ln) + 1);
	strcpy(person->lastName, ln);
	person->title = (char*)malloc(strlen(title) + 1);
	strcpy(person->title, title);
	person->age = age;
}

void deallocatePerson(Person *person){
	free(person->firstName);
	free(person->lastName);
	free(person->title);
}

Person *list[LIST_SIZE];

void initializeList(){
	int i;
	for(i = 0; i < LIST_SIZE; i++){
		list[i] = NULL;
	}
}

Person* getPerson(){
	int i;
	for(i = 0; i < LIST_SIZE; i++){
		if(list[i] != NULL){
			Person *ptr = list[i];
			list[i] = NULL;
			return ptr;
		}
	}
	Person *person = (Person*)malloc(sizeof(Person));
	return person;
}

void displayPerson(Person *person){
	printf("%s %s %s, %d\n", person->title, person->firstName, person->lastName, person->age);
}

Person* returnPerson(Person *person){
	int i;
	for(i = 0; i < LIST_SIZE; i++){
		if(list[i] == NULL){
			list[i] = person;
			deallocatePerson(list[i]);
			return person;
		}
	}
	deallocatePerson(person);
	free(person);
	return NULL;
}

void freeList(){
	int i;
	for(i = 0; i < LIST_SIZE; i++){
		free(list[i]);
	}
}

void processPerson(){
	Person person;
	initializePerson(&person, "Peter", "Underwood", "Manager", 36);
	deallocatePerson(&person);
}

typedef struct _emplyee{
	char name[32];
	unsigned char age;
}Employee;

int compareEmployee(Employee *e1, Employee *e2){
	return strcmp(e1->name, e2->name);
}

void displayEmployee(Employee *employee){
	printf("%s\t%d\n", employee->name, employee->age);
}

typedef void (*DISPLAY)(void*);
typedef int (*COMPARE)(void*, void*);

typedef struct _node{
	void *data;
	struct _node *next;
}Node;

typedef struct _linkedList{
	Node *head;
	Node *tail;
	Node *current;
}LinkedList;

void initializeLinkedList(LinkedList*);
void addHead(LinkedList*, void*);
void addTail(LinkedList*, void*);
void deleteNode(LinkedList*, Node*);
Node *getNode(LinkedList*, COMPARE, void*);
void displayLinkedList(LinkedList*, DISPLAY);

//typedef LinkedList Queue;
//void initializeQueue(Queue*);
//void enqueue(Queue*, void*);
//void* dequeue(Queue*);

typedef LinkedList Stack;
void initializeStack(Stack*);
void push(Stack*, void*);
void *pop(Stack*);

typedef struct _tree{
	void *data;
	struct _tree *left;
	struct _tree *right;
}TreeNode;

void insertNode(TreeNode**, COMPARE, void*);
void inOrder(TreeNode*, DISPLAY);
void postOrder(TreeNode*, DISPLAY);
void preOrder(TreeNode*, DISPLAY);
void freeTree(TreeNode* root){
	if(root != NULL){
		freeTree(root->left);
		freeTree(root->right);
		free(root);	
	}
}

int main(){
	
	Person person;
	initPerson(&person);
	person.firstName = (char*)malloc(strlen("Emily") + 1);
	strcpy(person.firstName, "Emily");
	person.age = 23;
	cleanPerson(&person);

	Person *ptrPerson;
	ptrPerson = (Person*)malloc(sizeof(Person));
	initPerson(ptrPerson);
	ptrPerson->firstName = (char*)malloc(strlen("Emily") + 1);
	strcpy(ptrPerson->firstName, "Emily");
	ptrPerson->age = 23;
	cleanPerson(ptrPerson);
	free(ptrPerson);

	Person person_;
	AlternatePerson otherPerson_;
	printf("%zu\n", sizeof(person_));
	printf("%zu\n", sizeof(otherPerson_));
	
	initializeList();
	Person *PtrPerson;
	PtrPerson = getPerson();
	initializePerson(PtrPerson, "Ralph", "Fitsgerald", "Mr.", 35);
	displayPerson(PtrPerson);
	returnPerson(PtrPerson);
	freeList();
	
	LinkedList linkedList;
	
	Employee *samuel = (Employee*)malloc(sizeof(Employee));
	strcpy(samuel->name, "Samuel");
	samuel->age = 32;
	
	Employee *sally = (Employee*)malloc(sizeof(Employee));
	strcpy(sally->name, "Sally");
	sally->age = 28;
	
	Employee *susan = (Employee*)malloc(sizeof(Employee));
	strcpy(susan->name, "Susan");
	susan->age = 45;
	
	initializeLinkedList(&linkedList);
	
//	addHead(&linkedList, samuel);
//	addHead(&linkedList, sally);
//	addHead(&linkedList, susan);
	addTail(&linkedList, samuel);
	addTail(&linkedList, sally);
	addTail(&linkedList, susan);
	displayLinkedList(&linkedList, (DISPLAY)displayEmployee);
	
	Node *node = getNode(&linkedList, (int (*)(void*, void*))compareEmployee, sally);
	deleteNode(&linkedList, node);
	displayLinkedList(&linkedList, (DISPLAY)displayEmployee);
	
	node = getNode(&linkedList, (int (*)(void*, void*))compareEmployee, samuel);
	deleteNode(&linkedList, node);
	node = getNode(&linkedList, (int (*)(void*, void*))compareEmployee, susan);
	deleteNode(&linkedList, node);
	printf("\n");

//	Queue queue;
//	initializeQueue(&queue);
//	
//	enqueue(&queue, samuel);
//	enqueue(&queue, sally);
//	enqueue(&queue, susan);
//	
//	void *data = dequeue(&queue);
//	printf("Dequeued %s\n", ((Employee*)data)->name);
//	data = dequeue(&queue);
//	printf("Dequeued %s\n", ((Employee*)data)->name);
//	data = dequeue(&queue);
//	printf("Dequeued %s\n", ((Employee*)data)->name);
	
	Stack stack;
	initializeStack(&stack);
	
	push(&stack, samuel);
	push(&stack, sally);
	push(&stack, susan);
	
	Employee *employee;
	
	int i;
	for(i = 0; i < 4; i++){
		employee = (Employee*)pop(&stack);
		printf("Popped %s\n", employee->name);
	}
	printf("\n");
	
	TreeNode *tree = NULL;
	insertNode(&tree, (COMPARE)compareEmployee, samuel);
	insertNode(&tree, (COMPARE)compareEmployee, sally);
	insertNode(&tree, (COMPARE)compareEmployee, susan);
	printf("inOrder:\n");
	inOrder(tree, (DISPLAY)displayEmployee);
	printf("\n");
	printf("postOrder:\n");
	postOrder(tree, (DISPLAY)displayEmployee);
	printf("\n");
	printf("preOrder:\n");
	preOrder(tree, (DISPLAY)displayEmployee);
	printf("\n");
	freeTree(tree);
	
	system("PAUSE");
	return 0;
}

void initializeLinkedList(LinkedList *list){
	list->head = NULL;
	list->tail = NULL;
	list->current = NULL;
}

void addHead(LinkedList *list, void *data){
	Node *node = (Node*)malloc(sizeof(Node));
	node->data = data;
	if(list->head == NULL){
		list->tail = node;
		node->next = NULL;
	}
	else{
		node->next = list->head;
	}
	list->head = node;
}

void addTail(LinkedList *list, void *data){
	Node *node = (Node*)malloc(sizeof(Node));
	node->data = data;
	node->next = NULL;
	if(list->head == NULL){
		list->head = node;
	}
	else{
		list->tail->next = node;
	}
	list->tail = node;
}

void deleteNode(LinkedList* list, Node* node){
	if(node == list->head){
		if(list->head->next == NULL){
			list->head = list->tail = NULL;
		}
		else{
			list->head = list->head->next;
		}
	}
	else{
		Node *tmp = list->head;
		while(tmp != NULL && tmp->next != node){
			tmp = tmp->next;
		}
		if(tmp != NULL){
			tmp->next = node->next;
		}
	}
	free(node);
}

Node *getNode(LinkedList *list, COMPARE compare, void *data){
	Node *node = list->head;
	while(node != NULL){
		if(compare(node->data, data) == 0){
			return node;
		}
		node = node->next;
	}
	return NULL;
}

void displayLinkedList(LinkedList *list, DISPLAY display){
	printf("\nLinked List\n");
	Node *current = list->head;
	while(current != NULL){
		display(current->data);
		current = current->next;
	}
}

//void initializeQueue(Queue *queue){
//	initializeList(queue);
//}
//
//void enqueue(Queue *queue, void *node){
//	addHead(queue, node);
//}
//
//void* dequeue(Queue *queue){
//	Node *tmp = queue->head;
//	void *data;
//	
//	if(queue->head == NULL){
//		data = NULL;
//	}
//	else if(queue->head == queue->tail){
//		queue->head = queue->tail = NULL;
//		data = tmp->data;
//		free(tmp);
//	}
//	else{
//		while(tmp->next != queue->tail){
//			tmp = tmp->next;
//		}
//		queue->tail = tmp;
//		tmp = tmp->next;
//		queue->tail->next = NULL;
//		data = tmp->data;
//		free(tmp);
//	}
//	
//	return data;
//}

void initializeStack(Stack *stack){
	initializeList(stack);
}

void push(Stack *stack, void *data){
	addHead(stack, data);
}

void *pop(Stack *stack){
	Node *node = stack->head;
	if(node == NULL){
		return NULL;
	}
	else if(node == stack->tail){
		stack->head = stack->tail = NULL;
		void *data = node->data;
		free(node);
		return data;
	}
	else{
		stack->head = stack->head->next;
		void *data = node->data;
		free(node);
		return data;
	}
}

void insertNode(TreeNode **root, COMPARE compare, void *data){
	TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	
	if(*root == NULL){
		*root = node;
		return;
	}
	
	while(1){
		if(compare((*root)->data, data) > 0){
			if((*root)->left != NULL){
				*root = (*root)->left;
			}
			else{
				(*root)->left = node;
				break;
			}
		}
		else{
			if((*root)->right != NULL){
				*root = (*root)->right;
			}
			else{
				(*root)->right = node;
				break;
			}
			
		} 
	}
}

void inOrder(TreeNode* root, DISPLAY display){
	if(root != NULL){
		inOrder(root->left, display);
		display(root->data);
		inOrder(root->right, display);
	}
}

void postOrder(TreeNode* root, DISPLAY display){
	if(root != NULL){
		postOrder(root->left, display);
		postOrder(root->right, display);
		display(root->data);	
	}
}

void preOrder(TreeNode* root, DISPLAY display){
	if(root != NULL){
		display(root->data);
		preOrder(root->left, display);
		preOrder(root->right, display);	
	}
}
