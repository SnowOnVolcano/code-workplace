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


int main() {
	int a;
	change(&a);
	printf("a = %d", a);

	init();
}