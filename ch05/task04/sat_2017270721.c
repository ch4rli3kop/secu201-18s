#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	int n_num; /*����� �ѹ�*/
	char data; /*����� ������*/
	node* l_child; node* r_child; node* parent;
	int value;
} node;

typedef struct NNandD {
	int n_num;
	char data;
}NNandD;

node* make_2_tree(NNandD* nnd, int m, int size) { /*����Ʈ���� ����ô�*/
												  /*m�� �Էµ� ����� ���� ���� ū ��*/
												  /*size�� �Էµ� ����� ��*/
	printf("����ڰ� �Է��� ���� ������ ����Ʈ���� ����ô� \n");
	int i, j;
	node* made_2_tree = (node *)malloc(sizeof(node)*m);
	int k = 1;
	for (j = 0; j < size; j++)
		printf("(nnd + %d)->n_num = %d \n", j, (nnd + j)->n_num);

	printf("max node num = %d \n", m);

	for (i = 1; i < m + 1; i++) {
		for (j = 0; j < size; j++) {
			if (i == (nnd + j)->n_num) {
				printf("Ʈ�� ��� %d(i��) �� ����� �Է� ��� %d( (nnd+j)->n_num��)�� ����. \n", i, (nnd + j)->n_num);
				(made_2_tree + i)->n_num = (nnd + j)->n_num;
				(made_2_tree + i)->data = (nnd + j)->data;
				printf("�Էµ� �����ʹ� %c \n", (nnd + j)->data);

				(made_2_tree + i)->l_child = (made_2_tree + 2 * i);
				((made_2_tree + i)->l_child)->data = '0';
				printf("%d��° ��ġ�� l_child�� ��ġ�� %d\n", i, 2 * i);


				(made_2_tree + i)->r_child = (made_2_tree + 2 * i + 1);
				((made_2_tree + i)->r_child)->data = '0';
				printf("%d��° ��ġ�� r_child�� ��ġ�� %d\n", i, 2 * i + 1);
				k++;
				printf("\n");
			}
		}
	}

	return made_2_tree + 1;
}

int mmm = 0;
void inorder(node* prt)
{

	if ((prt)->data != '0') {
		inorder(prt->l_child);
		inorder(prt->r_child);
		printf("-------'%c'---------\n", prt->data);
		switch (prt->data) {
		case 'T':
			prt->value = 1;
			mmm++;
			break;
		case 'F':
			prt->value = 0;
			mmm++;
			break;
		case'A':
			prt->value = prt->l_child->value && prt->r_child->value;
			mmm++;
			break;
		case'V':
			prt->value = prt->l_child->value || prt->r_child->value;
			mmm++;
			break;
		case '-':
			prt->value = !(prt->r_child->value);
			mmm++;
			break;
		}
	}
	if ((mmm == 11) ||  (mmm == 12))
		printf("����� %d \n", prt->value);
}


int main() {
	int size_T;
	printf("���� ����Ʈ���� ���� �Է� -> ");
	scanf_s("%d", &size_T);
	NNandD* NND = (NNandD*)malloc(sizeof(NNandD)*size_T);

	int max = 0;
	int n = 0;
	for (n = 0; n < size_T; n++) {
		printf("����� �ڸ��� ��忡 �� char�� �����͸� �Է��ϼ��� ->");
		scanf_s("%d", &(NND + n)->n_num);
		if ((NND + n)->n_num > max) max = (NND + n)->n_num;
		scanf_s("%c", &(NND + n)->data);
		printf("n = %d, ��� ��ġ�� %d, �Էµ� �����ʹ� %c \n", n, (NND + n)->n_num, (NND + n)->data);
	}

	node* kkk = make_2_tree(NND, max, size_T);

	inorder(kkk);

	printf("\n");
}
