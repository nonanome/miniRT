/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base_world.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:33:02 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/14 12:29:41 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

t_shape *new_shape(int type)
{
	t_shape	*shape;

	shape = (t_shape *)malloc(sizeof(t_shape));
	if (!shape)
		return (NULL);
	shape->type = type;
	shape->origin = set_vector(0, 0, 0, 1);
	shape->default_transformation = (double **)malloc(sizeof(double *) * 4);
	if (!shape->default_transformation)
		return (free(shape), NULL);
	shape->default_transformation = get_identity_matrix();
	shape->material = default_material();
	shape->id = globalID++;
	if (type == 0)
	{
		shape->radius = 1;
		shape->minimum = -INFINITY;
		shape->maximum = INFINITY;
		shape->closed = false;
	}
	else if (type == 1)
	{
		shape->normal = set_vector(0, 1, 0, 0);
		shape->minimum = -INFINITY;
		shape->maximum = INFINITY;
		shape->closed = false;
	}
	else if (type == 2)
	{
		shape->radius = 1;
		shape->minimum = -INFINITY;
		shape->maximum = INFINITY;
		shape->closed = false;
	}
	return (shape);
}

t_world	*get_world(int shapes)
{
	t_world	*world;

	world = (t_world *)malloc(sizeof(t_world));
	if (!world)
		return (NULL);
	world->canvas = (t_c *)calloc(sizeof(t_c), 1);
	if (!world->canvas)
		return (free(world), NULL);
	world->env = (t_env *)malloc(sizeof(t_env));
	if (!world->env)
		return (free(world->canvas), free(world), NULL);
	world->shapes = (t_shape **)malloc(sizeof(t_shape *) * shapes);
	if (!world->shapes)
		return (free(world->env), free(world->canvas), free(world), NULL);
	world->ambient = malloc(sizeof(xyzvektor));
	if (!world->ambient)
		return (free(world->shapes), free(world->env), free(world->canvas), free(world), NULL);
	*(world->ambient) = set_vector(0, 0, 0, 0);
	world->ambient_intensity = 0;
	world->nr_shapes = 0;
	world->all_sorted = NULL;
	return (world);
}

void	free_world(t_world *world)
{
	if (!world)
		return ;
	if (world->canvas)
		free(world->canvas->img);
	if (world->env)
		free(world->env);
	if (world->shapes)
		{
			while (world->nr_shapes--)
			{
			free(world->shapes[world->nr_shapes]->default_transformation);
			free(world->shapes[world->nr_shapes]);
			}
		}
	free(world);
}
