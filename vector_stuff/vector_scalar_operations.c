#include "../miniRT.h"
#include <stdio.h>

t_xyzvektor scalarMultiplication(t_xyzvektor a, double b)
{
	t_xyzvektor result;

	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;
	result.w = a.w * b;
	return result;
}

t_xyzvektor scalarDivision(t_xyzvektor a, double b)
{
	t_xyzvektor result;

	result.x = a.x / b;
	result.y = a.y / b;
	result.z = a.z / b;
	result.w = a.w / b;
	return result;
}

