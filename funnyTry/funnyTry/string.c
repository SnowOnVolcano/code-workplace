#include <stdio.h>

const int const_i_1 = 1;
const int const_i_2 = 2, const_i_3 = 3;
const char const_c_1 = 'a', const_c_2 = 'b';
const char const_c_3 = 'c';

int var_i_1, var_i_2, var_i_3;
char var_c_1, var_c_2, var_c_3;

void main() {
	/*const int local_const_i_1 = 100;
	const char local_const_c_1 = 'z';
	int local_i_1;
	int local_i_2;
	char local_c_1;
	char local_c_2;
	int expr1, expr2;

	scanf("%d%d", &local_i_1, &local_i_2);
	scanf("%d%d", &var_i_1, &var_i_2);

	printf("---Hello World--- input:  ");

	printf("   local_i_1 = %d", local_i_1);
	printf("   local_i_2 = %d", local_i_2);
	printf("   var_i_1 = %d", var_i_1);
	printf("   var_i_2 = %d", var_i_2);

	expr1 = local_i_1 + local_i_2 + local_const_i_1 - (local_const_c_1) / const_i_2;
	expr2 = -const_c_1 * (const_c_2 + const_c_3) - var_i_1 * ((var_i_2 - local_i_1) / local_i_2);
	printf("   expr1 = %d", expr1);
	printf("   expr2 = %d", expr2);*/
	int expr1;
	int expr2;
	int var1;
	char c = 'k';
	scanf("%d%d%d",&expr1, &expr2, &var1);
	expr1 = -(var1 - c) + 'd' - ('c') + 20 + 2 * expr1 - expr2 / 4 + 3;

	printf("%d\n", expr1);
}