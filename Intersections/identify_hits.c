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