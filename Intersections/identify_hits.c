#include "../miniRT.h"

xyzvektor identify_hit(t_all_intersec all_intersections)
{
	t_intersec hit_intersection;
	t_ray ray;
	double		time_of_hit;

	hit_intersection = all_intersections.intersections[0];
	ray = all_intersections.intersections[0].ray;
	time_of_hit = get_smallest_positive_value(&hit_intersection);
	return ray_position(ray, time_of_hit);
}

xyzvektor point_of_intersection(t_intersec *intersec, t_ray ray)
{
	double time_of_intersection;
	xyzvektor way;
	xyzvektor point_of_intersection;

	time_of_intersection = get_smallest_positive_value(intersec);
	way = scalarMultiplication(ray.direction , time_of_intersection);
	point_of_intersection = addition(ray.origin, way);
	return point_of_intersection;
}