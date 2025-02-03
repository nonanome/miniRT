#include "../miniRT.h"

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
	if(a.w == 1 || b.w == 1)
		result.w = 1;
	else
		result.w = 0;
	return result;
}

xyzvektor hadamard_product(xyzvektor a, xyzvektor b)
{
	xyzvektor result;

	result.x = a.x * b.x;
	result.y = a.y * b.y ;
	result.z = a.z * b.z;
	if(a.w == 1 || b.w == 1)
		result.w = 1;
	else
		result.w = 0;
	return result;
}
