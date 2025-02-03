/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base_world.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:33:02 by qhahn             #+#    #+#             */
/*   Updated: 2025/02/03 16:24:35 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "world.h"

t_world	*get_world(int spheres)
{
	t_world	*world;

	world = (t_world *)malloc(sizeof(t_world));
	if (!world)
		return (NULL);
	world->canvas = (t_c *)malloc(sizeof(t_c));
	if (!world->canvas)
		return (free(world), NULL);
	world->env = (t_env *)malloc(sizeof(t_env));
	if (!world->env)
		return (free(world->canvas), free(world), NULL);
	world->spheres = (t_sphere *)malloc(sizeof(t_sphere) * spheres);
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
	if (world->spheres)
		{
			while (world->nr_spheres--)
			{
			free(world->spheres[world->nr_spheres].default_transformation);
			
			}
		}
	free(world);
}
