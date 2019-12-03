#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include "trans.h"

int trans_A_main(int num)
{
	FILE* fp_r, * fp_w;
	MM_typecode matcode;
	int M, N, nz, ret;
	int i, * I, * J;
	double* val;

	char pathIn[0x100], pathOut[0x100];
	sprintf(pathIn, "%s%d%s", "circuit_", num, ".mtx");
	sprintf(pathOut, "%s%d%s", "circuit_", num, "_new.txt");
	fp_r = fopen(pathIn, "r");
	fp_w = fopen(pathOut, "w");
	if ((ret = mm_read_banner(fp_r, &matcode)) == 0)
		printf("Banner readin successfully!\n");
	else
		printf("!!!Banner readin fails! With ERRORCODE %d\n", ret);
	if ((ret = mm_read_mtx_crd_size(fp_r, &M, &N, &nz)) == 0)
		printf("Size readin successfully!\n");
	else
		printf("!!!Size readin fails! With ERRORCODE %d\n", ret);

	I = (int*)malloc(nz * sizeof(int));
	J = (int*)malloc(nz * sizeof(int));
	val = (double*)malloc(nz * sizeof(double));
	for (i = 0; i < nz; i++)
	{
		fscanf(fp_r, "%d %d %lg\n", &I[i], &J[i], &val[i]);
		//printf("%d...%d\n", I[i], J[i]);
		I[i]--;  /* adjust from 1-based to 0-based */
		J[i]--;
	}
	fclose(fp_r);

	mm_write_banner(fp_w, matcode);
	mm_write_mtx_crd_size(fp_w, M, N, nz);
	for (i = 0; i < nz; i++)
		fprintf(fp_w, "%d %d %d\n", I[i] + 1, J[i] + 1, (int)(val[i] * 1000000));
	fclose(fp_w);

	return 0;
}
