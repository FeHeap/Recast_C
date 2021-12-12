#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#define INPUT_FILE "test_case_3-3/input_3.txt"
#define OUTPUT_FILE "test_case_3-3/output_3.txt"
#define TRUE 1

FILE *fin, *fout;



typedef struct treeNode {
	int left_bound, right_bound;
	char color;
	int downFlag;
	struct treeNode *left, *right;
} TreeNode;

void instructionProcess(TreeNode* , char*);
TreeNode* buildNewTree(int, int);
void freeTree(TreeNode*);

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
	
	
	
	int wallLength;
	int numOfInst;
	char charBuff;
	fscanf(fin, "%d", &wallLength);
	charBuff = fgetc(fin);
	
	TreeNode *root = buildNewTree(0, wallLength);
	
	
	fscanf(fin, "%d", &numOfInst);
	charBuff = fgetc(fin);
	char instBuff[30];
	int i;

	for(i = 0; i < numOfInst; i++) {
		fscanf(fin, "%[^\n]", instBuff);
		//printf("%d %s\n", i, instBuff);
		charBuff = fgetc(fin);
		instructionProcess(root, instBuff);
	}
	
	
	freeTree(root);
	root = NULL;
	
	// close files
	fclose(fin);
	fclose(fout);
	
	return 0;
}

TreeNode* buildNewTree(int left_bound, int right_bound) {
	TreeNode *Node = (TreeNode*)malloc(sizeof(TreeNode));
	Node->left_bound = left_bound;
	Node->right_bound = right_bound;
	Node->color = '_';
	Node->downFlag = 0;
	Node->left = NULL;
	Node->right = NULL;
	if(left_bound != right_bound) {
		int mid = (left_bound + right_bound) / 2;
		Node->left = buildNewTree(left_bound, mid);
		Node->right = buildNewTree(mid + 1, right_bound);
	}
	return Node;
}

void freeTree(TreeNode *Node) {
	if(Node->left == NULL && Node->left == NULL) {
		free(Node);
	}
	else {
		freeTree(Node->left);
		freeTree(Node->right);
	}
}

void query(TreeNode*, char*);
void paint(TreeNode*, char*);

void printTree(TreeNode *root) {
	if(root->downFlag == 1) {
		printTree(root->left);
		printTree(root->right);
	}
	else {
		printf("%d%c%d ", root->left_bound, root->color, root->right_bound);	
	}
	
}

void instructionProcess(TreeNode *root, char *instruction) {
	int  i = 0;
	while(!isspace(instruction[i++]));
	switch(instruction[0]) {
		case 'Q':
			query(root, &instruction[i]);
			break;
		case 'P':
			paint(root, &instruction[i]);
			break;
		default: // pass
			break;
	}
}


int strToInt(char *str) {
	int returnInt = 0;	
	int i;
	for(i = 0; TRUE; i++) {
		if(isspace(str[i]) || str[i] == '\0') {
			break;
		}
		returnInt *= 10;
		returnInt += (str[i] - '0');
	}
	return returnInt;
}


char rightState = 0;
void queryColor(TreeNode *root, int left_q, int right_q) {
	if(root->downFlag == 0) {
		if(rightState == 0) {
			fprintf(fout, "%c", root->color);
			rightState = root->color;
		}
		else {
			if(rightState != root->color) {
				fprintf(fout, " %c", root->color);
				rightState = root->color;
			}
		}
	}
	else {
		int mid = (root->left_bound + root->right_bound) / 2;
		if(root->left_bound <= left_q && left_q <= mid || root->left_bound <= right_q && right_q <= mid || left_q <= root->left_bound && mid <= right_q) {
			queryColor(root->left, left_q, right_q);
		}
		if(mid+1 <= right_q && right_q <= root->right_bound || mid+1 <= left_q && left_q <= root->right_bound || left_q <= mid+1 && root->right_bound <= right_q) {
			queryColor(root->right, left_q, right_q);
		}
	}
}

void query(TreeNode *root, char *instruction) {
	int left_q, right_q;
	left_q = strToInt(instruction);
	int  i = 0;
	while(!isspace(instruction[i++]));
	right_q = strToInt(instruction+i);
	rightState = 0;
	queryColor(root, left_q, right_q);
	fprintf(fout, "\n");
}

void paintColor(TreeNode *root, int left_p, int right_p, char color) {
	if(left_p <= root->left_bound && root->right_bound <= right_p || left_p > root->right_bound || right_p < root->left_bound) {
		root->color = color;
		root->downFlag = 0;
	}
	else {
		int nowDownFlag = root->downFlag;
		if(nowDownFlag == 0) {
			root->left->color = root->color;
			root->left->downFlag = 0;
			root->right->color = root->color;
			root->right->downFlag = 0;
		}
		root->downFlag = 1;
		int mid = (root->left_bound + root->right_bound) / 2;
		char event = 0;
		if(root->left_bound <= left_p && left_p <= mid || root->left_bound <= right_p && right_p <= mid || left_p <= root->left_bound && mid <= right_p) {
			paintColor(root->left, left_p, right_p, color);
			event += 'l';
		}
		if(mid+1 <= right_p && right_p <= root->right_bound || mid+1 <= left_p && left_p <= root->right_bound || left_p <= mid+1 && root->right_bound <= right_p) {
			paintColor(root->right, left_p, right_p, color);
			event += 'r';
		}
		
		if(nowDownFlag == 0) {
			if(event == 'l') {
				paintColor(root->right, left_p, right_p, root->color);
			}
			else if(event == 'r') {
				paintColor(root->left, left_p, right_p, root->color);
			}
		}
	}
}

void paint(TreeNode *root, char *instruction) {
	int left_p, right_p;
	left_p = strToInt(instruction);
	int  i = 0;
	while(!isspace(instruction[i++]));
	right_p = strToInt(instruction+i);
	while(!isspace(instruction[i++]));
	char color = instruction[i];
	paintColor(root, left_p, right_p, color);
}

