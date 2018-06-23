#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define SIZE 50
#define STACK_SIZE 50


//token   
typedef enum {
	false, true, lparen, and, or , rparen, not, end
} logical;
// false, true, lparen, and, or, rparen, not, eos

typedef struct _node *treePointer;
typedef struct _node {
	logical data;
	short int value;
	treePointer leftChild; 
	treePointer rightChild;
} node;



treePointer save_ptr[100];		// save formula
treePointer save_post[100];		// save to postfix
treePointer stack[STACK_SIZE];	// stack
treePointer root = NULL;		// root

// in-stack precedence, incoming precedence ( lparen, rparen )
//const int isp[] = {0, 0, 0, 1, 2, 3, 4, 0} 
//const int icp[] = {0, 0, 5, 1, 2, 3, 4, 0}
// p.143
const int isp[] = { 0, 0, 0, 1, 2, 3, 4, 0 };
const int icp[] = { 0, 0, 5, 1, 2, 3, 4, 0 };

// variable space
int var_space[SIZE];

void pushStack(int *top, treePointer item);
treePointer popStack(int *top);
void postorderEval(treePointer ptr);
void convert_to_word(char *ptr);
logical getToken(treePointer *str_arr, treePointer *symbol, int *n);
void postfix();
void convert_to_tree();
void printToken();
void var_cnt(char *str, int *space, int *cnt);
void all_of_situation(int *var_space, char *str, int cnt);
int power(int x, int y);




int main() {
	int cnt = 0, round;
	char str[100];			// input string
	char cpy[100];
	printf("input any formula [ex. Av(B^-C)] : \n");

	gets(str);
	strcpy(cpy, str);
	printf("String = %s\n", cpy);

	var_cnt(str, var_space, &cnt);
	
	round = power(2, cnt);


	while(round--){
		all_of_situation(var_space, cpy, cnt);
		printf("%s\n", cpy);
		convert_to_word(cpy);
		postfix();
		printToken();
		convert_to_tree();
		postorderEval(root);

		int result = root->value;
		if (result)
			printf("\nresult = TRUE\n\n");
		else
			printf("\nresult = FALSE\n\n");

	}
	return 0;
}


int power(int x, int y) {
	int result = 1;
	for (int i = 0; i < y; i++) {
		result *= x;
	}
	return result;
}


void var_cnt(char *str, int *space, int *cnt) {
	do {
		switch (*str) {
		case '-':
		case '^':
		case 'v':
		case '(':
		case ')':
			break;
		default:
			*space++ = 1;
			(*cnt)++;
		}
	} while (*++str != NULL);
	printf("number of var = %d\n\n", *cnt);

}

void all_of_situation(int *var_space, char *str, int cnt) {
	int round = 0;
	for (int i = 0, j = 0; str[i] != NULL; i++) {
		switch (str[i]) {
		case '-':
		case '^':
		case 'v':
		case '(':
		case ')':
			break;
		default:
			str[i] = var_space[j++] + '0';
		}
	}

	int k = 0;

	for (int i = 0; i < cnt; i++)
		k |= var_space[i] << (cnt - 1) - i;
	k -= 1;
	for (int i = 0; i < cnt; i++) {
		var_space[i] = (k & power(2, (cnt - 1) - i))
			>> ((cnt - 1) - i);
	}
}


void pushStack(int *top, treePointer item) {
	if (*top == STACK_SIZE) {
		printf("stack full\n");
		exit(0);
	}
	stack[++(*top)] = item;
}

treePointer popStack(int *top) {
	if (*top == -1) {
		return end;
	}
	return stack[(*top)--];
}
// 후위 순회(p.230)
void postorderEval(treePointer ptr) {
	if (ptr) {
		postorderEval(ptr->leftChild);
		postorderEval(ptr->rightChild);

		switch (ptr->data) {
		case not:
			ptr->value = !ptr->rightChild->value;
			break;
		case and:
			ptr->value = ptr->rightChild->value &&
				ptr->leftChild->value;
			break;
		case or :
			ptr->value = ptr->rightChild->value ||
				ptr->leftChild->value;
			break;
		case true:
			ptr->value = TRUE;
			break;
		case false:
			ptr->value = FALSE;
			break;
		}
	}
}
// 입력한 식을 각각의 노드에 넣어줌
void convert_to_word(char *ptr) {
	int cnt = 0;
	while (ptr[cnt] != NULL) {
		save_ptr[cnt] = (treePointer)malloc(sizeof(node));
		switch (ptr[cnt]) {
		case '0':
			save_ptr[cnt]->data = false;
			break;
		case '1':
			save_ptr[cnt]->data = true;
			break;
		case '^':
			save_ptr[cnt]->data = and;
			break;
		case 'v':
			save_ptr[cnt]->data = or ;
			break;
		case '-':
			save_ptr[cnt]->data = not;
			break;
		case '(':
			save_ptr[cnt]->data = lparen;
			break;
		case ')':
			save_ptr[cnt]->data = rparen;
			break;
		default:
			printf("error\n");
			exit(0);
		
			//break;
		}
		save_ptr[cnt]->value = 0;
		save_ptr[cnt]->leftChild = NULL;
		save_ptr[cnt]->rightChild = NULL;
		cnt++;
	}
	save_ptr[cnt] = (treePointer)malloc(sizeof(node));
	save_ptr[cnt]->data = end;
	save_ptr[cnt]->value = 0;
	save_ptr[cnt]->leftChild = NULL;
	save_ptr[cnt]->rightChild = NULL;
}

logical getToken(treePointer *str_arr,treePointer *symbol, int *n) {
	*symbol = str_arr[(*n)++];
	switch ((*symbol)->data) {
	case and:
	case or:
	case not:
	case true:
	case false:
	case lparen:
	case rparen:
		return (*symbol)->data;
	default:
		return end;
	}
}

// 수식을 후위 표기식으로 저장 (p.144)
void postfix() {
	treePointer symbol;
	treePointer *str = save_post;

	logical token;

	int n = 0;
	int top = 0;
	

	stack[0] = (treePointer)malloc(sizeof(node));
	(stack[0])->data = end;
	(stack[0])->leftChild = NULL;
	(stack[0])->rightChild = NULL;
	(stack[0])->value = 0;


	for (token = getToken(save_ptr, &symbol, &n); token != end; token = getToken(save_ptr, &symbol, &n)) {
		if (token == true || token == false)
			*(str++) = symbol;
		else if (token == rparen) {
			while (stack[top]->data != lparen) 
				*str++ = popStack(&top);
			popStack(&top);
		}
		else {
			while (isp[stack[top]->data] > icp[token]) 
				*str++ = popStack(&top);
			pushStack(&top, symbol);
		}
	}
	while (top > -1) 
		*str++ = popStack(&top);
}

void convert_to_tree() {
	logical token;
	treePointer symbol;
	treePointer op1, op2;

	int n = 0;
	int top = -1;

	token = getToken(save_post, &symbol, &n);
	while (token != end) {
		if (token == true || token == false || symbol == root)
			pushStack(&top, symbol);
		else {
			if (token == not) {
				op1 = popStack(&top);
				symbol->rightChild = op1;
			}
			else {
				op2 = popStack(&top);
				op1 = popStack(&top);
				symbol->leftChild = op1;
				symbol->rightChild = op2;
			}
			root = symbol;
			pushStack(&top, root);
		}
		token = getToken(save_post, &symbol, &n);
	}
	if (top == 0)
		root = stack[top];
}

void printToken() {
	treePointer *str = save_post;

	printf("postfix : ");
	while (*str != NULL) {
		switch ((*str)->data) {
		case false:
			printf("F "); *str++;
			break;
		case true:
			printf("T "); *str++;
			break;
		case not:
			printf("-"); *str++;
			break;
		case and:
			printf("^"); *str++;
			break;
		case or:
			printf("v"); *str++;
			break;
		default:
			*str++;
		}
	}
}

