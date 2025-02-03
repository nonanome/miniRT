/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_world.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:20:42 by qhahn             #+#    #+#             */
/*   Updated: 2025/02/03 16:13:53 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "world.h"

static void	save_intersections(t_c *canvas, t_intersec *new_intersection, t_world *world)
{
	int	i;

	i = 0;
	if (canvas->all_intersections.nr_intersections == 0)
	{
		canvas->all_intersections.intersections = malloc(sizeof(t_intersec));
		if (!canvas->all_intersections.intersections)
			exit(1);
	}
	else
	{
		canvas->all_intersections.intersections = realloc(canvas->all_intersections.intersections,
				sizeof(t_intersec) * (canvas->all_intersections.nr_intersections
					+ 1));
		if (!canvas->all_intersections.intersections)
			exit(1);
	}
	canvas->all_intersections.intersections[canvas->all_intersections.nr_intersections] = *new_intersection;
	canvas->all_intersections.nr_intersection_entries++;
	while (new_intersection->times[i] != 0)
	{
		if (canvas->all_intersections.nr_intersections %100 >= 90)
		{
			world->all_sorted = realloc(world->all_sorted, sizeof(double *) * (world->canvas->all_intersections.nr_intersections + 100));
			if (!world->all_sorted)
				exit(1);
		}
		world->all_sorted[world->canvas->all_intersections.nr_intersections + i] = new_intersection->times[i];
		i++;
	}
	canvas->all_intersections.nr_intersections += i;
}

int	intersect_world(t_world *world, t_ray ray)
{
	t_intersec	*new_intersection;
	int			i;
	int			j;

	i = 0;
	j = 0;
	while (world->nr_spheres > i)
	{
		new_intersection = intersect(world->spheres[i], ray);
		if (new_intersection)
		{
			save_intersections(world->canvas, new_intersection, world);
		}
		i++;
	}
	return (0);
}
