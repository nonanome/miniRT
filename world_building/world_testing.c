/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_testing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:20:56 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/22 19:21:32 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

int	main(void)
{
	t_world			*world;
	t_ray			ray;
	t_comp			comp;
	t_xyzvektor		shadestuff;
	double			**retmatrix;
	double			**rot;
	double			**trans;
	mlx_image_t		*image;
	unsigned int	color;
	t_xyzvektor		color2;

	world = get_world(6);
	if (!world)
		return (1);
	init_canvas(world->canvas);
	world->all_sorted = calloc(sizeof(double *), 100);
	if (!world->all_sorted)
		return (1);
	world->all_sorted[0] = 0;
	world->camera = camera(world->canvas->width, world->canvas->height, PI / 3);
	if (!world->camera)
	{
		free_world(world);
		return (1);
	}
	if (parse_input("test.rt", world))
	{
		free_world(world);
		return (1);
	}
	if (!world->camera->transform)
		return (1);
	mlx_delete_image(world->canvas->mlx_ptr, image);
	image = render_image(world->camera, world);
	if (!image)
	{
		free_world(world);
		return (1);
	}
	mlx_image_to_window(world->canvas->mlx_ptr, image, 0, 0);
	mlx_loop(world->canvas->mlx_ptr);
	free_world(world);
	return (0);
}
