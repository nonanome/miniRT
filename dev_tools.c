#include "miniRT.h"

void show_matrix(double **a)
{
	int i = 0;
	int j = 0;

	while(i < 4)
	{
		while(j < 4)
		{
			printf("%f ", a[i][j]);
			j ++;
		}
		printf("\n");
		j = 0;
		i ++;
	}
}

void show_vector(xyzvektor a)
{
	printf("%f %f %f %f", a.x, a.y, a.z, a.w);
}