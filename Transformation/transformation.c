#include "../miniRT.h"

t_ray transform(t_ray ray, double **transformation)
{
	t_ray new_ray;

	new_ray.origin = multiply_vector_and_matrix(ray.origin, transformation);
	new_ray.direction = multiply_vector_and_matrix(ray.direction, transformation);
	return new_ray;
}

void set_transform(t_shape *shape, double **translation)
{
	shape->default_transformation = translation;
}

