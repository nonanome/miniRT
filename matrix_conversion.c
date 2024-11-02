#include "miniRT.h"



double **transpose_matrix(double **a, int size)
{
	double **result;
	int i;
	int j;

	result = malloc(size * (sizeof(double *)));
	i = -1;
	while(++ i < size)
		result[i] = malloc(size * (sizeof(double)));
	i = 0;
	j = 0;
	while(i < size)
	{
		while(j < size)
		{
			result[i][j] = a[j][i];
			j ++;
		}
		j = 0;
		i ++;
	}
	return result;
}

double get_minor(double **a, int i, int j, int size)
{
	double **submatrix;
	double determinante;

	submatrix = get_submatrix(a, i, j, size);
	determinante = get_determinante_of_2(submatrix);
	free_double_ptr(submatrix, 2);
	return (determinante);
}

double get_determinante_of_2(double **a)
{
	return a[0][0] * a[1][1] - a[1][0] * a[0][1];
}

double *extract_submatrix_row(double *a, int ii, int size)
{
	double *row;
	int i;
	int j;

	i = 0;
	j = 0;
	row = malloc((size - 1) * (sizeof(double)));
	while(i < size)
	{
		if (i != ii)
		{
			row[j] = a[i];
			j ++;
		}
		i ++;	
	}
	return row;
}

double **get_submatrix(double **a, int ii, int jj, int size)
{
	double **result;
	int i;
	int j;

	result = malloc((size - 1) * (sizeof(double *)));
	i = 0;
	while(i < (size))
	{
		if (i != ii)
		{
			result[j] = extract_submatrix_row(a[i], jj, size);
			j ++;
		}	
		i ++;
	}
	return result;
}





double **invert_matrix(double **a, int size)
{
	double **result;
	int i;
	int j;

	result = malloc(size * (sizeof(double *)));
	i = -1;
	while(++ i < size)
		result[i] = malloc(size * (sizeof(double)));
	i = 0;
	j = 0;
	while(i < size)
	{
		while(j < size)
		{
			result[i][j] = 1 / a[i][j];
			j ++;
		}
		j = 0;
		i ++;
	}
	return result;
}

double get_cofactor(double **a, int i, int j, int size)
{
	if((i + j) % 2 != 0)
		return -get_minor(a, i, j, size);
	return get_minor(a, i, j , size);
}

double get_determinant_of_bigger_matrix(double **a, int size)
{
	double result;
	int i;

	result = 0;
	i = 0;
	while(i < size)
	{
		result = result + get_cofactor(a, 0, i, 3) * a[0][i];
		i ++;
	}
	return result;
}

void show_matrix(double **a, int size)
{
	int i = 0;
	int j = 0;

	while(i < size)
	{
		while(j < size)
		{
			printf("%f ", a[i][j]);
			j ++;
		}
		printf("\n");
		j = 0;
		i ++;
	}
}

void free_double_ptr(double **a, int size)
{
	int i;

	i = 0;
	while(i < size)
	{
		free(a[i]);
		i ++;
	}
	free(a);
}






int main(void)
{
	double **a;
	int i;
	int j;

	a = malloc(3 * (sizeof(double *)));
	i = -1;
	while(++ i < 3)
		a[i] = malloc(3 * (sizeof(double)));
	
	a[0][0] = 1;
	a[0][1] = 2;
	a[0][2] = 6;
	a[1][0] = -5;
	a[1][1] = 8;
	a[1][2] = -4;
	a[2][0] = 2;
	a[2][1] = 6;
	a[2][2] = 4;

	printf("%f", get_determinant_of_bigger_matrix(a, 3));
	// get_cofactor(a, 0, 0, 3);
	// get_cofactor(a, 0, 1, 3);
	// get_cofactor(a, 0, 2, 3);
	// printf("%f",get_minor(a, 0, 0, 3));
	//show_matrix(get_submatrix(a, 0, 0, 3), 2);
}
