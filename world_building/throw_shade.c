/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   throw_shade.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:14:50 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/27 19:50:52 by qhahn            ###   ########.fr       */
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
			hit_intersection = calculate_hit_point(all_intersections.intersections[i],
					t0);
			if (t0 < 0 || t0 > t1)
			{
				hit_intersection = calculate_hit_point(all_intersections.intersections[i],
						t1);
			}
			return (hit_intersection);
		}
		i++;
	}
	return (hit_intersection);
}

bool	*is_shadowed(t_world *world, t_xyzvektor point, t_shape shape)
{
	t_ray		ray;
	t_xyzvektor	v;
	double		distance;
	t_xyzvektor	hit_intersection;
	int			i;
	bool		*shadows;
	double		rotation[3][3];
	double		**inverse_transform;

	i = -1;
	shadows = malloc(world->canvas->num_lights * sizeof(bool));
	ray = init_ray();
	ray.origin = point;
	while (++i < world->canvas->num_lights)
	{
		v = substraction(world->canvas->lightsource[i].position, point);
		distance = magnitude(v);
		ray.direction = normalize(v);
		if (shape.type != 0)
		{
			inverse_transform = invert_matrix(shape.default_transformation, 4);
			// Transform the ray into the object's local space
			transform_ray(&ray, inverse_transform);
		}
		empty_intersections(world->canvas);
		intersect_world(world, ray);
		if (world->canvas->all_intersections.intersections != NULL)
			hit_intersection = hit(world->canvas->all_intersections);
		else
			hit_intersection.w = INFINITY;
		if (hit_intersection.w > EPSILON && hit_intersection.w < distance)
			shadows[i] = true;
		else
			shadows[i] = false;
	}
	empty_intersections(world->canvas);
	return (shadows);
}

t_xyzvektor	shade_hit(t_world *world, t_comp comp, t_shape shape)
{
	t_xyzvektor	retvalue;
	t_c			local_canvas;
	bool		*in_shadow;

	local_canvas = *(world->canvas);
	local_canvas.normale = comp.normalv;
	local_canvas.eyevector = comp.eyev;
	in_shadow = is_shadowed(world, comp.over_point, shape);
	empty_intersections(world->canvas);
	return (lightning(*(comp.object), comp.over_point, local_canvas,
			in_shadow));
}
