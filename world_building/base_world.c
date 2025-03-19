/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base_world.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkuhn <kkuhn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:33:02 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/18 20:06:34 by kkuhn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

t_shape *new_shape(int type)
{
	t_shape	*shape;

	shape = (t_shape *)MALLOC(sizeof(t_shape));
	if (!shape)
		return (NULL);
	shape->type = type;
	shape->origin = set_vector(0, 0, 0, 1);
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

	world = (t_world *)MALLOC(sizeof(t_world));
	if (!world)
		return (NULL);
	world->canvas = (t_c *)calloc(sizeof(t_c), 1);
	if (!world->canvas)
		return (FREE(world), NULL);
	world->env = (t_env *)MALLOC(sizeof(t_env));
	if (!world->env)
		return (FREE(world->canvas), FREE(world), NULL);
	world->shapes = (t_shape **)MALLOC(sizeof(t_shape *) * shapes);
	if (!world->shapes)
		return (FREE(world->env), FREE(world->canvas), FREE(world), NULL);
	world->ambient = MALLOC(sizeof(xyzvektor));
	if (!world->ambient)
		return (FREE(world->shapes), FREE(world->env), FREE(world->canvas), FREE(world), NULL);
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
		FREE(world->canvas->img);
	if (world->env)
		FREE(world->env);
	if (world->ambient)
		FREE(world->ambient);
	if (world->camera)
	{
		free_double_ptr(world->camera->transform, 4);
		FREE(world->camera);
	}
	if (world->shapes)
		{
			while (world->nr_shapes--)
			{
				free_double_ptr(world->shapes[world->nr_shapes]->default_transformation, 4);
				FREE(world->shapes[world->nr_shapes]);
			}
		}
	FREE(world->shapes);
	FREE(world->canvas);
	FREE(world->all_sorted);
	FREE(world);
}
