/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   throw_shade.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:14:50 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/22 17:57:33 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

void	empty_intersections(t_c *canvas)
{
	int	i;

	i = 0;
	if (canvas->all_intersections.intersections)
	{
		while (i < canvas->all_intersections.nr_intersection_entries)
			FREE(canvas->all_intersections.intersections[i++].times);
		FREE(canvas->all_intersections.intersections);
		canvas->all_intersections.nr_intersection_entries = 0;
		canvas->all_intersections.nr_intersections = 0;
	}
	canvas->all_intersections.intersections = NULL;
}

static t_xyzvektor	calculate_hit_point(t_intersec intersection, double time)
{
	t_xyzvektor	result;

	result = addition(intersection.ray.origin,
			scalar_multiplication(intersection.ray.direction, time));
	result.w = time;
	return (result);
}

t_xyzvektor	hit(t_all_intersec all_intersections)
{
	t_xyzvektor	hit_intersection;
	double		t0;
	double		t1;
	int			i;

	i = 0;
	hit_intersection = set_vector(0, 0, 0, 0);
	while (i <= all_intersections.nr_intersections / 2 - 1)
	{
		t0 = all_intersections.intersections[i].times[0];
		t1 = all_intersections.intersections[i].times[1];
		if (t0 > 0 && t0 < t1)
		{
			hit_intersection = calculate_hit_point
				(all_intersections.intersections[i], t0);
			if (t0 < 0 || t0 > t1)
			{
				hit_intersection = calculate_hit_point
					(all_intersections.intersections[i], t1);
			}
			return (hit_intersection);
		}
		i++;
	}
	return (hit_intersection);
}

bool	is_shadowed(t_world *world, t_xyzvektor point)
{
	t_ray		ray;
	t_xyzvektor	v;
	double		distance;
	t_xyzvektor	hit_intersection;

	v = substraction(world->canvas->lightsource.position, point);
	distance = magnitude(v);
	ray = init_ray();
	ray.origin = point;
	ray.direction = normalize(v);
	empty_intersections(world->canvas);
	intersect_world(world, ray);
	if (world->canvas->all_intersections.intersections != NULL)
		hit_intersection = hit(world->canvas->all_intersections);
	if (hit_intersection.w > EPSILON && hit_intersection.w < distance)
		return (empty_intersections(world->canvas), true);
	return (empty_intersections(world->canvas), false);
}

t_xyzvektor	shade_hit(t_world *world, t_comp comp)
{
	t_xyzvektor	retvalue;
	t_c			local_canvas;
	bool		in_shadow;

	local_canvas = *(world->canvas);
	local_canvas.normale = comp.normalv;
	local_canvas.eyevector = comp.eyev;
	in_shadow = true;
	in_shadow = is_shadowed(world, comp.over_point);
	empty_intersections(world->canvas);
	return (lightning(*(comp.object), comp.over_point, local_canvas,
			in_shadow));
}
