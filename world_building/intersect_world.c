/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_world.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:20:42 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/22 17:45:47 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

void	find_nearest_intersection(t_intersec *intersections,
		int nr_intersections, t_intersec **intersec_to_use, int *shape_to_use)
{
	int	i;

	i = 0;
	while (i < nr_intersections)
	{
		if (intersections[i].times[0] > 0)
		{
			if (!*intersec_to_use)
			{
				*intersec_to_use = &intersections[i];
				*shape_to_use = intersections[i].object_id;
			}
			else if (intersections[i].times[0] < (*intersec_to_use)->times[0])
			{
				*intersec_to_use = &intersections[i];
				*shape_to_use = intersections[i].object_id;
			}
		}
		i++;
	}
}

t_xyzvektor	color_at(t_world *world, t_ray ray)
{
	t_comp		comp;
	t_intersec	*intersec_to_use;
	int			shape_to_use;

	if (world->canvas->all_intersections.intersections)
	{
		world->canvas->all_intersections.intersections = NULL;
		world->canvas->all_intersections.nr_intersections = 0;
	}
	world->canvas->all_intersections.nr_intersection_entries = 0;
	intersec_to_use = NULL;
	shape_to_use = 0;
	intersect_world(world, ray);
	if (world->canvas->all_intersections.nr_intersections == 0)
		return (set_black());
	find_nearest_intersection(world->canvas->all_intersections.intersections,
		world->canvas->all_intersections.nr_intersection_entries,
		&intersec_to_use, &shape_to_use);
	if (!intersec_to_use)
		return (set_black());
	comp = prepare_computations(intersec_to_use, ray,
			(world->shapes[shape_to_use]));
	empty_intersections(world->canvas);
	return (shade_hit(world, comp));
}

static void	sort_intersections(double *all_sorted)
{
	int		i;
	int		j;
	double	temp;

	i = 0;
	while (all_sorted[i] != 0)
	{
		j = i + 1;
		while (all_sorted[j] != 0)
		{
			if (all_sorted[i] > all_sorted[j])
			{
				temp = all_sorted[i];
				all_sorted[i] = all_sorted[j];
				all_sorted[j] = temp;
			}
			j++;
		}
		i++;
	}
}

t_comp	prepare_computations(t_intersec *intersection, t_ray ray,
		t_shape *shape)
{
	t_comp	comps;

	comps.t = intersection->times[0];
	comps.object = shape;
	comps.point = point_of_intersection(intersection, ray);
	comps.eyev = negate_tuple(ray.direction);
	comps.normalv = calculate_normale(*shape, comps.point);
	comps.over_point = set_vector(comps.point.x + comps.normalv.x * EPSILON,
			comps.point.y + comps.normalv.y * EPSILON, comps.point.z
			+ comps.normalv.z * EPSILON, 1);
	if (dot_product(comps.normalv, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normalv = negate_tuple(comps.normalv);
	}
	else
		comps.inside = false;
	return (comps);
}

int	intersect_world(t_world *world, t_ray ray)
{
	t_intersec	*new_intersection;
	int			i;
	int			j;

	i = 0;
	j = 0;
	while (world->nr_shapes > i)
	{
		new_intersection = intersect(world->shapes[i], ray);
		if (new_intersection)
			save_intersections(world->canvas, new_intersection, world);
		i++;
	}
	sort_intersections(world->all_sorted);
	return (0);
}
