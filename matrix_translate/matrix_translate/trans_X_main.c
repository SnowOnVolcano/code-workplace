#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include "trans.h"

int trans_X_main(int num)
{
	FILE* fp_r, * fp_w;
	MM_typecode matcode;
	int M, N, nz, ret;
	int i, * I, * J;
	double* val;

	char pathIn[0x100], pathOut[0x100];
	sprintf(pathIn, "%s%d%s", "circuit_", num, "_x.mtx");
	sprintf(pathOut, "%s%d%s", "circuit_", num, "_x_new.txt");
	fp_r = fopen(pathIn, "r");
	fp_w = fopen(pathOut, "w");
	if ((ret = mm_read_banner(fp_r, &matcode)) == 0)
		printf("Banner readin successfully!\n");
	else
		printf("!!!Banner readin fails! With ERRORCODE %d\n", ret);
	if ((ret = mm_read_mtx_array_size(fp_r, &M, &N)) == 0)
		printf("Size readin successfully!\n");
	else
		printf("!!!Size readin fails! With ERRORCODE %d\n", ret);

	//I = (int *) malloc(nz * sizeof(int));
	//J = (int *) malloc(nz * sizeof(int));
	val = (double*)malloc(M * N * sizeof(double));
	for (i = 0; i < M * N; i++)
	{
		fscanf(fp_r, "%lg\n", &val[i]);
		//printf("%d...%d\n", I[i], J[i]);
		//I[i]--;  /* adjust from 1-based to 0-based */
		//J[i]--;
	}
	fclose(fp_r);

	mm_write_banner(fp_w, matcode);
	mm_write_mtx_array_size(fp_w, M, N);
	for (i = 0; i < M * N; i++)
		fprintf(fp_w, "%d\n", (int)(val[i] * 1000000));
	fclose(fp_w);

	return 0;
}
