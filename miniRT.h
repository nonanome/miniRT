#ifndef MINI_RT
#define MINI_RT
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft/libft.h"
# include "MLX42/include/MLX42/MLX42.h"

typedef struct s_koord
{
	double	x;
	double	y;
	double	z;
	double	w;
}	xyzvektor;

//trasnformation
double **translation(double x, double y, double z);
double **scaling(double x, double y, double z);
double **rotation_x(double degree);
double **rotation_y(double degree);
double **rotation_z(double degree);
double **shearing(double *proportions);

//vector operations
xyzvektor crossProduct(xyzvektor a, xyzvektor b);
double dotProduct(xyzvektor a, xyzvektor b);
xyzvektor substraction(xyzvektor a, xyzvektor b);
xyzvektor addition(xyzvektor a, xyzvektor b);
double magnitude(xyzvektor a);
xyzvektor normalize(xyzvektor a);
xyzvektor negateTuple(xyzvektor a);
xyzvektor scalarMultiplication(xyzvektor a, double b);
xyzvektor scalarDivision(xyzvektor a, double b);
xyzvektor set_vector(double x, double y, double z, double w);

//matrix operations
double **multiply_matrix(double **a, double **b);
xyzvektor	multiply_vector_and_matrix(xyzvektor a, double **b);
double **get_identity_matrix(void);
int compare_matrix(double **a, double **b, int size);
double **transpose_matrix(double **a, int size);
double get_minor(double **a, int i, int j, int size);
double get_determinante_of_2(double **a);
double *extract_submatrix_row(double *a, int ii, int size);
double **get_submatrix(double **a, int ii, int jj, int size);
double **invert_matrix(double **a, int size);
double get_cofactor(double **a, int i, int j, int size);
double get_determinant_of_bigger_matrix(double **a, int size);
double get_determinante_of_3x3(double **a);

//color operations
uint32_t	get_color_from_tuple(xyzvektor color);

//utils

void free_double_ptr(double **a, int size);
void show_matrix(double **a, int size);



#endif