#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
using namespace std;
int main()
{
	int a, b;
	scanf("%d", &a);
	scanf("%d", &b);
	char text_b[4];

	sprintf(text_b, "%d", b); //b�� ���� text_b �迭�� str�� ����ȯ ����

	int num = 0;
	for (int i = 2; i >= 0; i--) {
		num = text_b[i];
		printf("%d\n", a * (num - '0'));
	}
	printf("%d\n", a * b);
}
