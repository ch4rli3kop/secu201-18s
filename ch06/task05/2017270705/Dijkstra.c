#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 8
#define Inf 10000

typedef struct node *nodePointer;
typedef struct node {
	int vertex;
	nodePointer link;
}node;

// ��� ����
nodePointer path[MAX_VERTICES];

// p.320 ���� ���� ���
int cost[][MAX_VERTICES] = {
	0,		Inf,	Inf,	Inf,	Inf,	Inf,	Inf,	Inf,
	300,	0,		Inf,	Inf,	Inf,	Inf,	Inf,	Inf,
	1000,	800,	0,		Inf,	Inf,	Inf,	Inf,	Inf,
	Inf,	Inf,	1200,	0,		Inf,	Inf,	Inf,	Inf,
	Inf,	Inf,	Inf,	1500,	0,		250,	Inf,	Inf,
	Inf,	Inf,	Inf,	1000,	Inf,	0,		900,	1400,
	Inf,	Inf,	Inf,	Inf,	Inf,	Inf,	0,		1000,
	1700,	Inf,	Inf,	Inf,	Inf,	Inf,	Inf,	0
};

int distance[MAX_VERTICES];
short int found[MAX_VERTICES];
int n = MAX_VERTICES;

int choose(int distance[], int n, short int found[]);
void shortestPath(int v, int cost[][MAX_VERTICES], int distance[], int n, short int found[]);
void FREE(nodePointer *ptr);


int main()
{
	nodePointer temp;

	// ��� ��ο� ���� �ּҺ�� �̾Ƴ�
	for (int i = 0; i < n; i++) {
		shortestPath(i, cost, distance, n, found);
		for (int j = 0; j < n; j++) {
			printf("(%d) -> (%d) : ", i, j);
  			for (temp = path[j]; temp; temp = temp->link)
				printf("v%d ", temp->vertex);
			if (!path[j])
				printf("(NO path)\n\n");
			else
				printf("v%d\n\t\t\t\t%d\n", j, distance[j]);
			
		}
		printf("\n");
		for (int k = 0; k < n; k++) 
			FREE(&path[k]);
	}
	return 0;
}

void FREE(nodePointer *ptr) {
	nodePointer temp;
	while (*ptr) {
		temp = *ptr;
		*ptr = (*ptr)->link;
		free(temp);
	}
}


// p.318(�ϳ��� ��������� �ִ� ���)
// d[i] : ���� v���� i���� �ִ� ���, �ִ� ��� �߰��ϸ� 1�� found[i] = 1
void shortestPath(int v, int cost[][MAX_VERTICES], int distance[], int n, short int found[]) {
	int i, u, w;
	nodePointer ptr, temp, next;

	for (i = 0; i < n; i++) {
		found[i] = FALSE;
		distance[i] = cost[v][i];
	}
	found[v] = TRUE;
	distance[v] = 0;

	for (i = 0; i < n - 2; i++) {
		u = choose(distance, n, found);
		found[u] = TRUE;
		
		// �߰ߵ��� ���� vertex
		if (!path[u] && !(distance[u] == Inf)) {
			temp = (nodePointer)malloc(sizeof(node));
			temp->vertex = v;
			temp->link = NULL;
			path[u] = temp;
		}
		for (w = 0; w < n; w++) {
			if (!found[w])
				if (distance[u] + cost[u][w] < distance[w]) {
					distance[w] = distance[u] + cost[u][w];
					if (path[w]) {
						FREE(&path[w]);
					}
					for (ptr = path[u]; ptr; ptr = ptr->link) {
						temp = (nodePointer)malloc(sizeof(node));
						temp->vertex = ptr->vertex;
						temp->link = NULL;
						if (!path[w])
							path[w] = next = temp;
						else {
							next->link = temp;
							next = next->link;
						}
					}
					temp = (nodePointer)malloc(sizeof(struct node));
					temp->vertex = u;
					temp->link = NULL;
					next->link = temp;
				}
		}
	}
}


// p.319(���� ��� ������ ����)
// ���� ����ȵ� ���� �߿��� distance ���� �ּ��� ���� ã�� �Լ�
int choose(int distance[], int n, short int found[]) {
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for (i = 0; i<n; i++)
		if (distance[i] < min && !found[i]) {
			min = distance[i];
			minpos = i;
		}
	return minpos;
}

