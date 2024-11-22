#include "../miniRT.h"

double magnitude(xyzvektor a)
{
	double result;

	result = sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	return result;
}

xyzvektor normalize(xyzvektor a)
{
	xyzvektor result;

	result.x = a.x * magnitude(a);
	result.y = a.y * magnitude(a);
	result.z = a.z * magnitude(a);
	result.w = a.w * magnitude(a);
	return result;
}

xyzvektor negateTuple(xyzvektor a)
{
	xyzvektor result;

	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;
	result.w = -a.w;
	return result;
}

xyzvektor set_vector(double x, double y, double z, double w)
{
	xyzvektor return_vector;

	return_vector.x = x;
	return_vector.y = y;
	return_vector.z = z;
	return_vector.w = w;
	return return_vector;
}