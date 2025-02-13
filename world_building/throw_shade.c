/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   throw_shade.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:14:50 by qhahn             #+#    #+#             */
/*   Updated: 2025/02/03 17:30:16 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

void	empty_intersections(t_c *canvas)
{
	if (canvas->all_intersections.intersections)
	{
		free(canvas->all_intersections.intersections);
		canvas->all_intersections.intersections = NULL;
		canvas->all_intersections.nr_intersections = 0;
		canvas->all_intersections.nr_intersection_entries = 0;
	}
}

xyzvektor	hit(t_all_intersec all_intersections)
{
	int			i;
	xyzvektor	hit_intersection;

	i = 0;
	hit_intersection = set_vector(0, 0, 0, 0);
	while (i <= all_intersections.nr_intersections /2 -1)
	{
		if (all_intersections.intersections[i].times[0] > 0)
		{
			hit_intersection = all_intersections.intersections[i].ray.origin;
			hit_intersection = addition(hit_intersection, scalarMultiplication(all_intersections.intersections[i].ray.direction,
						all_intersections.intersections[i].times[0]));
			hit_intersection.w = all_intersections.intersections[i].times[0];
			return (hit_intersection);
		}
		i++;
	}
	return (hit_intersection);
}

bool	is_shadowed(t_world *world, xyzvektor point)
{
	t_ray		ray;
	xyzvektor	v;
	double		distance;
	xyzvektor	hit_intersection;

	v = substraction(world->canvas->lightsource.position, point);
	distance = magnitude(v);
	ray = init_ray();
	ray.origin = point;
	ray.direction = normalize(v);
	empty_intersections(world->canvas);
	intersect_world(world, ray);
	hit_intersection = hit(world->canvas->all_intersections);
	if (hit_intersection.w > 0 && hit_intersection.w < distance)
		return (true);
	return (false);
}

xyzvektor shade_hit(t_world *world, t_comp comp)
{
	xyzvektor retvalue;
	t_c local_canvas;
	bool in_shadow;

	local_canvas = *(world->canvas);
	local_canvas.normale = comp.normalv;
	local_canvas.eyevector = comp.eyev;
	in_shadow = is_shadowed(world, comp.over_point);
	return (lightning(comp.object->material, comp.over_point, local_canvas, in_shadow));
}
