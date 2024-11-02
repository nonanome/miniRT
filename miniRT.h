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





#endif