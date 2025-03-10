#include "../miniRT.h"


double **translation(double x, double y, double z)
{
	double **result;

	result = get_identity_matrix();
	result[0][3] = x;
	result[1][3] = y;
	result[2][3] = z;
	return result;
}

double **scaling(double x, double y, double z)
{
	double **result;

	int i = -1;
	result = ft_calloc(4 , (sizeof(double *)));
	while (++ i < 4)
		result[i] = ft_calloc(4 , (sizeof(double)));
	result[0][0] = x;
	result[1][1] = y;
	result[2][2] = z;
	result[3][3] = 1;
	return result;
}

double **rotation_x(double degree)
{
	double radians;
	double **result;

	radians = degree / 180 * PI;
	result = get_identity_matrix();
	result[1][1] = cos(radians);
	result[1][2] = -sin(radians);
	result[2][1] = sin(radians);	
	result[2][2] = cos(radians);
	return result;
}

double **rotation_y(double degree)
{
	double radians;
	double **result;

	radians = degree / 180 * PI;
	result = get_identity_matrix();
	result[0][0] = cos(radians);
	result[0][2] = sin(radians);
	result[2][0] = -sin(radians);
	result[2][2] = cos(radians);
	return result;
}

double **rotation_z(double radians)
{
	// double radians;
	double **result;

	// radians = degree / 180 * M_PI;
	result = get_identity_matrix();
	result[0][0] = cos(radians);
	result[0][1] = -sin(radians);
	result[1][0] = sin(radians);	
	result[1][1] = cos(radians);
	return result;
}

double **shearing(double *proportions)
{
	double **result;

	result = get_identity_matrix();
	result[0][1] = proportions[0];
	result[0][2] = proportions[1];
	result[1][0] = proportions[2];
	result[1][2] = proportions[3];
	result[2][0] = proportions[4];
	result[2][1] = proportions[5];
	return result ;
}



