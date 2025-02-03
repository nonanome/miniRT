#include "../miniRT.h"
#include <stdio.h>

xyzvektor scalarMultiplication(xyzvektor a, double b)
{
	xyzvektor result;

	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;
	result.w = a.w * b;
	return result;
}

xyzvektor scalarDivision(xyzvektor a, double b)
{
	xyzvektor result;

	result.x = a.x / b;
	result.y = a.y / b;
	result.z = a.z / b;
	result.w = a.w / b;
	return result;
}

