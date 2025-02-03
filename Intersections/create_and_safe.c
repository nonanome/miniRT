#include "../miniRT.h"

double get_discriminant(double *discriminant_values)
{
	return (discriminant_values[1] * discriminant_values[1] - 4
		* discriminant_values[0] * discriminant_values[2]);
}

t_intersec *intersect(t_sphere sphere, t_ray ray)
{
	xyzvektor sphere_to_ray;
	t_intersec	*result;
	double *discriminant_values;
	double discriminant;

	result = malloc(sizeof(t_intersec));
	ray = transform(ray, invert_matrix(sphere.default_transformation, 4));
	result->ray = ray;
	discriminant_values = malloc(3 * sizeof(double));
	sphere_to_ray = substraction(ray.origin, sphere.origin);
	discriminant_values[0] = dotProduct(ray.direction, ray.direction);
	discriminant_values[1] = 2 * dotProduct(ray.direction, sphere_to_ray);
	discriminant_values[2] = dotProduct(sphere_to_ray, sphere_to_ray) - sphere.radius * sphere.radius;
	discriminant = get_discriminant(discriminant_values);
	if(discriminant < 0)
		return NULL;
	result->times = malloc (2 * sizeof(double));
	result->times[0] = (-discriminant_values[1] - sqrt(discriminant)) / (2 * discriminant_values[0]);
	result->times[1] = (-discriminant_values[1] + sqrt(discriminant)) / (2 * discriminant_values[0]);
	result->object_id = sphere.id;
	return result;
}
