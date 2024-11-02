#include "miniRT.h"

xyzvektor addition(xyzvektor a, xyzvektor b)
{
	xyzvektor result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	result.w = a.w + b.w;
	return result;
}

xyzvektor substraction(xyzvektor a, xyzvektor b)
{
	xyzvektor result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	result.w = a.w - b.w;
	return result;
}


double dotProduct(xyzvektor a, xyzvektor b)
{
	double result;

	result = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	return result;
}

xyzvektor crossProduct(xyzvektor a, xyzvektor b)
{
	xyzvektor result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return result;
}

xyzvektor hadamard_product(xyzvektor a, xyzvektor b)
{
	xyzvektor result;

	result.x = a.x * b.x;
	result.y = a.y * b.y ;
	result.z = a.z * b.z;
	return result;
}
