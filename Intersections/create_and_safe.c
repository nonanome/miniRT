#include "../miniRT.h"

double get_discriminant(double *discriminant_values)
{
	return (discriminant_values[1] * discriminant_values[1] - 4
		* discriminant_values[0] * discriminant_values[2]);
}

t_intersec *intersect_sphere(t_intersec *result, t_ray ray, t_shape sphere) {
    double *discriminant_values;
    double discriminant;
    xyzvektor sphere_to_ray;

    discriminant_values = malloc(3 * sizeof(double));
    sphere_to_ray = substraction(ray.origin, sphere.origin);
    discriminant_values[0] = dotProduct(ray.direction, ray.direction);
    discriminant_values[1] = 2 * dotProduct(ray.direction, sphere_to_ray);
    discriminant_values[2] = dotProduct(sphere_to_ray, sphere_to_ray) - sphere.radius * sphere.radius;
    discriminant = get_discriminant(discriminant_values);
    if (discriminant < 0) {
        free(discriminant_values);
        return NULL;
    }
    result->times = malloc(2 * sizeof(double));
    result->times[0] = (-discriminant_values[1] - sqrt(discriminant)) / (2 * discriminant_values[0]);
    result->times[1] = (-discriminant_values[1] + sqrt(discriminant)) / (2 * discriminant_values[0]);
    result->object_id = sphere.id;
    free(discriminant_values);
    return result;
}

t_intersec *intersect_plane(t_intersec *result, t_ray ray, t_shape plane) {
    double *discriminant_values;
    double discriminant;

    discriminant_values = malloc(3 * sizeof(double));
    discriminant_values[0] = dotProduct(ray.direction, plane.normal);
    if (discriminant_values[0] == 0) {
        free(discriminant_values);
        return NULL;
    }
    result->times = malloc(2 * sizeof(double));
    discriminant_values[0] = dotProduct(ray.direction, plane.normal);
    discriminant_values[1] = -dotProduct(ray.origin, plane.normal);
    if (discriminant_values[1] / discriminant_values[0] < EPSILON) {
        free(discriminant_values);
        result->times[0] = 0;
        result->times[1] = 0;
        return result;
    }
    result->times[0] = discriminant_values[1] / discriminant_values[0];
    result->times[1] = discriminant_values[1] / discriminant_values[0];
    result->object_id = plane.id;
    free(discriminant_values);
    return result;
}

t_intersec *local_intersect(t_intersec *result, t_ray ray, t_shape *shape) {
    if (shape->type == 0) {
        return intersect_sphere(result, ray, *shape);
    } else if (shape->type == 1) {
        return intersect_plane(result, ray, *shape);
    }
    // else if (shape->type == 2) {
    //     return intersect_cylinder(result, ray, *shape);
	return NULL;
}

t_intersec *intersect(t_shape *shape, t_ray ray) {
    t_intersec *result;

    result = malloc(sizeof(t_intersec));
    ray = transform(ray, invert_matrix(shape->default_transformation, 4));
    result->ray = ray;
    return local_intersect(result, ray, shape);
}
