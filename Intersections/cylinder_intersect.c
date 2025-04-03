/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:12:29 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/03 19:27:20 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	create_rotation_matrix(t_xyzvektor normal, double **rotation)
{
	double	axis[3] = {0.0, 1.0, 0.0};
	double	dot;
	double	cross[3] = {0, 0, 0};
	double	sin_theta;
	double	cos_theta;

	dot = normal.x * axis[0] + normal.y * axis[1] + normal.z * axis[2];
	cross[0] = normal.y * axis[2] - normal.z * axis[1];
	cross[1] = normal.z * axis[0] - normal.x * axis[2];
	cross[2] = normal.x * axis[1] - normal.y * axis[0];
	sin_theta = sqrt(cross[0] * cross[0] + cross[1] * cross[1] + cross[2]
			* cross[2]);
	cos_theta = dot;
	rotation[0][0] = cos_theta + cross[0] * cross[0] * (1 - cos_theta);
	rotation[0][1] = cross[0] * cross[1] * (1 - cos_theta) - cross[2]
		* sin_theta;
	rotation[0][2] = cross[0] * cross[2] * (1 - cos_theta) + cross[1]
		* sin_theta;
	rotation[1][0] = cross[1] * cross[0] * (1 - cos_theta) + cross[2]
		* sin_theta;
	rotation[1][1] = cos_theta + cross[1] * cross[1] * (1 - cos_theta);
	rotation[1][2] = cross[1] * cross[2] * (1 - cos_theta) - cross[0]
		* sin_theta;
	rotation[2][0] = cross[2] * cross[0] * (1 - cos_theta) - cross[1]
		* sin_theta;
	rotation[2][1] = cross[2] * cross[1] * (1 - cos_theta) + cross[0]
		* sin_theta;
	rotation[2][2] = cos_theta + cross[2] * cross[2] * (1 - cos_theta);
}

void	transform_ray(t_ray *ray, double **rotation)
{
	double	origin[3] = {ray->origin.x, ray->origin.y, ray->origin.z};
	double	direction[3] = {ray->direction.x, ray->direction.y,
			ray->direction.z};

	ray->origin.x = rotation[0][0] * origin[0] + rotation[0][1] * origin[1]
		+ rotation[0][2] * origin[2];
	ray->origin.y = rotation[1][0] * origin[0] + rotation[1][1] * origin[1]
		+ rotation[1][2] * origin[2];
	ray->origin.z = rotation[2][0] * origin[0] + rotation[2][1] * origin[1]
		+ rotation[2][2] * origin[2];
	ray->direction.x = rotation[0][0] * direction[0] + rotation[0][1]
		* direction[1] + rotation[0][2] * direction[2];
	ray->direction.y = rotation[1][0] * direction[0] + rotation[1][1]
		* direction[1] + rotation[1][2] * direction[2];
	ray->direction.z = rotation[2][0] * direction[0] + rotation[2][1]
		* direction[1] + rotation[2][2] * direction[2];
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

void	cut_cone(t_intersec *result, t_ray ray, t_shape shape)
{
	double	tmp;

	if (result->times[0] > result->times[1])
	{
		tmp = result->times[0];
		result->times[0] = result->times[1];
		result->times[1] = tmp;
	}
	tmp = ray.origin.y + result->times[0] * ray.direction.y;
	if (0 > tmp || (shape.maximum) < tmp)
		result->times[0] = -1;
	return ;
}

// void caps(t_intersec *result, t_ray ray, t_shape shape)
// {
//     if (fabs(ray.direction.y) > EPSILON)
//     {
//         double t_top = (shape.maximum - ray.origin.y) / ray.direction.y;
//         double x = ray.origin.x + t_top * ray.direction.x;
//         double z = ray.origin.z + t_top * ray.direction.z;
//         if (x * x + z * z <= 1.0)
//         {
//             if (result->times[0] == -1 || t_top < result->times[0])
//             {
//                 result->times[1] = result->times[0];
//                 result->times[0] = t_top;
//             }
//             else if (result->times[1] == -1 || t_top < result->times[1])
//             {
//                 result->times[1] = t_top;
//             }
//         }
//     }
// }

void	cap_top(t_intersec *result, t_ray ray, t_shape shape)
{
	double	t_top;
	double	x_top;
	double	z_top;

	if (fabs(ray.direction.y) > EPSILON)
	{
		t_top = (shape.maximum - ray.origin.y) / ray.direction.y;
		x_top = ray.origin.x + t_top * ray.direction.x;
		z_top = ray.origin.z + t_top * ray.direction.z;
		if (x_top * x_top + z_top * z_top <= 1.0)
		{
			if (result->times[0] == -1 || t_top < result->times[0])
			{
				result->times[1] = result->times[0];
				result->times[0] = t_top;
			}
			else if (result->times[1] == -1 || t_top < result->times[1])
			{
				result->times[1] = t_top;
			}
		}
	}
}

void	cap_bottom(t_intersec *result, t_ray ray, t_shape shape)
{
	double	t_bottom;
	double	x_bottom;
	double	z_bottom;

	t_bottom = (shape.minimum - ray.origin.y) / ray.direction.y;
	x_bottom = ray.origin.x + t_bottom * ray.direction.x;
	z_bottom = ray.origin.z + t_bottom * ray.direction.z;
	if (x_bottom * x_bottom + z_bottom * z_bottom <= 1.0)
	{
		if (result->times[0] == -1 || t_bottom < result->times[0])
		{
			result->times[1] = result->times[0];
			result->times[0] = t_bottom;
		}
		else if (result->times[1] == -1 || t_bottom < result->times[1])
		{
			result->times[1] = t_bottom;
		}
	}
}

t_intersec	*cylinder_intersect(t_intersec *result, t_ray ray, t_shape cylinder)
{
	double	*discriminant_values;
	double	discriminant;
	double	**rotation;

	rotation = MALLOC(sizeof(double *) * 3);
	rotation[0] = MALLOC(sizeof(double) * 3);
	rotation[1] = MALLOC(sizeof(double) * 3);
	rotation[2] = MALLOC(sizeof(double) * 3);
	create_rotation_matrix(cylinder.normal, rotation);
	transform_ray(&ray, rotation);
	discriminant_values = MALLOC(3 * sizeof(double));
	result->times = MALLOC(2 * sizeof(double));
	discriminant_values[0] = ray.direction.x * ray.direction.x + ray.direction.z
		* ray.direction.z;
	if (discriminant_values[0] > -EPSILON && discriminant_values[0] < EPSILON)
		return (FREE(discriminant_values), NULL);
	discriminant_values[1] = 2 * ray.origin.x * ray.direction.x + 2
		* ray.origin.z * ray.direction.z;
	discriminant_values[2] = ray.origin.x * ray.origin.x + ray.origin.z
		* ray.origin.z - 1;
	discriminant = discriminant_values[1] * discriminant_values[1] - 4
		* discriminant_values[0] * discriminant_values[2];
	if (discriminant < 0)
		return (FREE(discriminant_values), NULL);
	result->times[0] = (-discriminant_values[1] - sqrt(discriminant)) / (2
			* discriminant_values[0]);
	result->times[1] = (-discriminant_values[1] + sqrt(discriminant)) / (2
			* discriminant_values[0]);
	result->object_id = cylinder.id;
	cut_cylinder(result, ray, cylinder);
	if (result->times[0] == -1)
		cap_top(result, ray, cylinder);
	if (result->times[1] == -1)
		cap_bottom(result, ray, cylinder);
	FREE(discriminant_values);
	return (result);
}

t_intersec	*cone_intersect(t_intersec *result, t_ray ray, t_shape cone)
{
	double	*discriminant_values;
	double	discriminant;
	double	**rotation;
	double	tan_theta;

	rotation = MALLOC(sizeof(double *) * 3);
	rotation[0] = MALLOC(sizeof(double) * 3);
	rotation[1] = MALLOC(sizeof(double) * 3);
	rotation[2] = MALLOC(sizeof(double) * 3);
	tan_theta = cone.radius / (cone.maximum - cone.minimum);
	create_rotation_matrix(cone.normal, rotation);
	// ray.origin.y -= cone.maximum;
	transform_ray(&ray, rotation);
	// ray.origin.y -= cone.maximum;
	discriminant_values = MALLOC(3 * sizeof(double));
	result->times = MALLOC(2 * sizeof(double));
	discriminant_values[0] = ray.direction.x * ray.direction.x + ray.direction.z
		* ray.direction.z - ray.direction.y * ray.direction.y * tan_theta
		* tan_theta;
	if (discriminant_values[0] > -EPSILON && discriminant_values[0] < EPSILON)
		return (FREE(discriminant_values), NULL);
	discriminant_values[1] = 2 * ray.origin.x * ray.direction.x + 2
		* ray.origin.z * ray.direction.z - 2 * ray.origin.y * ray.direction.y
		* tan_theta * tan_theta;
	discriminant_values[2] = ray.origin.x * ray.origin.x + ray.origin.z
		* ray.origin.z - ray.origin.y * ray.origin.y * tan_theta * tan_theta;
	discriminant = discriminant_values[1] * discriminant_values[1] - 4
		* discriminant_values[0] * discriminant_values[2];
	if (discriminant < 0)
		return (FREE(discriminant_values), NULL);
	result->times[0] = (-discriminant_values[1] - sqrt(discriminant)) / (2
			* discriminant_values[0]);
	result->times[1] = (-discriminant_values[1] + sqrt(discriminant)) / (2
			* discriminant_values[0]);
	result->object_id = cone.id;
	cut_cylinder(result, ray, cone);
	if (result->times[0] == -1)
		cap_top(result, ray, cone);
	if (result->times[1] == -1)
		cap_bottom(result, ray, cone);
	FREE(discriminant_values);
	return (result);
}
