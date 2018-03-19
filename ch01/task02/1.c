#include <stdio.h>

void hanoi_tower(int i, char from, char tmp, char to);

int main(void)

{
	int n;
	scanf("%d", &n);
	hanoi_tower(n, 'A', 'B', 'C');//����Լ�

}

void hanoi_tower(int i, char from, char tmp, char to)

{

	if (i == 1)

		printf("�����̵� :1�� %c���� %c���� �̵�!\n", from, to);

	else

	{

		hanoi_tower(i - 1, from, to, tmp);//from, to, tmp�� ��ġ ����

		printf("�����̵� :%d�� %c���� %c���� �̵�!\n", i, from, to);

		hanoi_tower(i - 1, tmp, from, to);//tmp, from, to�� ��ġ ����

	}

}


