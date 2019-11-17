#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int change(int* a) {
	*a = 6;
	return 1;
}


void change_array(char* a) {
	strcpy(a, "222");
}

void init() {
	char* ddd = (char*)malloc(sizeof(char) * 0x40);
	change_array(ddd);
	puts(ddd);
}


int a1 = 1;
int a2 = 10;
int b[10];
int c[10];

int main() {
	//int a;
	// change(&a);
	// printf("a = %d", a);

	b[0] = c[0] = 111;
	b[1] = c[1] = 999;

	b[a1--] = b[a1];
	c[--a1] = c[a1];

	for (int i = 0; i < 2; i++) {
		printf("b[%d] = %d, ", i, b[i]);
	}
	for (int i = 0; i < 2; i++) {
		printf("c[%d] = %d, ", i, c[i]);
	}
}