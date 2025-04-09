/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:12:29 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/09 17:27:02 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	create_rotation_matrix(t_xyzvektor normal, double **rotation)
{
	double	axis[3];
	double	dot;
	double	c[3];
	double	sin_theta;
	double	cos_theta;
	double	u[3];

	axis[0] = 0.0;
	axis[1] = 1.0;
	axis[2] = 0.0;
	dot = normal.x * axis[0] + normal.y * axis[1] + normal.z * axis[2];
	c[0] = normal.y * axis[2] - normal.z * axis[1];
	c[1] = normal.z * axis[0] - normal.x * axis[2];
	c[2] = normal.x * axis[1] - normal.y * axis[0];
	sin_theta = sqrt(c[0] * c[0] + c[1] * c[1] + c[2] * c[2]);
	if (sin_theta > EPSILON)
	{
		u[0] = c[0] / sin_theta;
		u[1] = c[1] / sin_theta;
		u[2] = c[2] / sin_theta;
	}
	else
	{
		u[0] = u[1] = u[2] = 0.0;
	}
	cos_theta = dot;
	rotation[0][0] = cos_theta + u[0] * u[0] * (1 - cos_theta);
	rotation[0][1] = u[0] * u[1] * (1 - cos_theta) - u[2] * sin_theta;
	rotation[0][2] = u[0] * u[2] * (1 - cos_theta) + u[1] * sin_theta;
	rotation[1][0] = u[1] * u[0] * (1 - cos_theta) + u[2] * sin_theta;
	rotation[1][1] = cos_theta + u[1] * u[1] * (1 - cos_theta);
	rotation[1][2] = u[1] * u[2] * (1 - cos_theta) - u[0] * sin_theta;
	rotation[2][0] = u[2] * u[0] * (1 - cos_theta) - u[1] * sin_theta;
	rotation[2][1] = u[2] * u[1] * (1 - cos_theta) + u[0] * sin_theta;
	rotation[2][2] = cos_theta + u[2] * u[2] * (1 - cos_theta);
	rotation[0][3] = 0.0;
	rotation[1][3] = 0.0;
	rotation[2][3] = 0.0;
	rotation[3][0] = 0.0;
	rotation[3][1] = 0.0;
	rotation[3][2] = 0.0;
	rotation[3][3] = 1.0;
}

void	transform_ray(t_ray *ray, double **matrix)
{
	double	ox;
	double	oy;
	double	oz;
	double	ow;
	double	nx;
	double	ny;
	double	nz;
	double	nw;
	double	dx;
	double	dy;
	double	dz;
	double	dw;

	// origin is a point: w=1
	ox = ray->origin.x;
	oy = ray->origin.y;
	oz = ray->origin.z;
	ow = 1.0;
	nx = matrix[0][0] * ox + matrix[0][1] * oy + matrix[0][2] * oz
		+ matrix[0][3] * ow;
	ny = matrix[1][0] * ox + matrix[1][1] * oy + matrix[1][2] * oz
		+ matrix[1][3] * ow;
	nz = matrix[2][0] * ox + matrix[2][1] * oy + matrix[2][2] * oz
		+ matrix[2][3] * ow;
	nw = matrix[3][0] * ox + matrix[3][1] * oy + matrix[3][2] * oz
		+ matrix[3][3] * ow;
	if (nw != 0.0)
	{
		nx /= nw;
		ny /= nw;
		nz /= nw;
	}
	ray->origin.x = nx;
	ray->origin.y = ny;
	ray->origin.z = nz;
	dx = ray->direction.x;
	dy = ray->direction.y;
	dz = ray->direction.z;
	dw = 0.0;
	nx = matrix[0][0] * dx + matrix[0][1] * dy + matrix[0][2] * dz
		+ matrix[0][3] * dw;
	ny = matrix[1][0] * dx + matrix[1][1] * dy + matrix[1][2] * dz
		+ matrix[1][3] * dw;
	nz = matrix[2][0] * dx + matrix[2][1] * dy + matrix[2][2] * dz
		+ matrix[2][3] * dw;
	nw = matrix[3][0] * dx + matrix[3][1] * dy + matrix[3][2] * dz
		+ matrix[3][3] * dw;
	ray->direction.x = nx;
	ray->direction.y = ny;
	ray->direction.z = nz;
}

void	cut_cylinder(t_intersec *result, t_ray ray, t_shape shape)
{
	double	tmp;

	if (result->times[0] > result->times[1])
	{
		tmp = result->times[0];
		result->times[0] = result->times[1];
		result->times[1] = tmp;
	}
	tmp = ray.origin.y + result->times[0] * ray.direction.y;
	if (shape.minimum > tmp || shape.maximum < tmp)
		result->times[0] = -1;
	tmp = ray.origin.y + result->times[1] * ray.direction.y;
	if ((shape.minimum > tmp || shape.maximum < tmp))
		result->times[1] = -1;
	return ;
}

double	get_discriminant(double *discriminant_values, t_ray ray,
		t_shape cylinder, t_intersec *result)
{
	double	discriminant;

	discriminant_values[0] = ray.direction.x * ray.direction.x + ray.direction.z
		* ray.direction.z;
	if (discriminant_values[0] > -EPSILON && discriminant_values[0] < EPSILON)
		return (FREE(discriminant_values), -1);
	discriminant_values[1] = 2 * ray.origin.x * ray.direction.x + 2
		* ray.origin.z * ray.direction.z;
	discriminant_values[2] = ray.origin.x * ray.origin.x + ray.origin.z
		* ray.origin.z - 1;
	discriminant = discriminant_values[1] * discriminant_values[1] - 4
		* discriminant_values[0] * discriminant_values[2];
	if (discriminant < 0)
		return (FREE(discriminant_values), -1);
	result->times[0] = (-discriminant_values[1] - sqrt(discriminant)) / (2
			* discriminant_values[0]);
	result->times[1] = (-discriminant_values[1] + sqrt(discriminant)) / (2
			* discriminant_values[0]);
	result->object_id = cylinder.id;
	return (discriminant);
}

t_intersec	*cylinder_intersect(t_intersec *result, t_ray ray, t_shape cylinder)
{
	double	*discriminant_values;
	double	discriminant;
	double	**rotation;

	discriminant_values = MALLOC(3 * sizeof(double));
	result->times = MALLOC(2 * sizeof(double));
	discriminant = get_discriminant(discriminant_values, ray, cylinder, result);
	if (discriminant == -1)
		return (NULL);
	cut_cylinder(result, ray, cylinder);
	if (result->times[0] == -1)
		cap_top(result, ray, cylinder);
	if (result->times[1] == -1)
		cap_bottom(result, ray, cylinder);
	FREE(discriminant_values);
	return (result);
}
