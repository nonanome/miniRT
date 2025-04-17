/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:20:56 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/17 18:39:35 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world_building/world.h"

#define PI 3.14159265358979323846

t_xyzvektor	ray_position(t_ray ray, double time)
{
	t_xyzvektor	result;

	result = addition(ray.origin, scalar_multiplication(ray.direction, time));
	return (result);
}

int	main(int argc, char *argv[])
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

	world = get_world(100);
	if (!world || argc != 2)
		return (1);
	init_canvas(world->canvas);
	world->all_sorted = ft_calloc(sizeof(double *), 100);
	if (!world->all_sorted)
		return (1);
	world->all_sorted[0] = 0;
	if (parse_input(argv[1], world))
	{
		free_world(world);
		return (1);
	}
	if (!world->camera->transform)
		return (1);
	image = render_image(world->camera, world);
	if (!image)
	{
		free_world(world);
		return (1);
	}
	mlx_image_to_window(world->canvas->mlx_ptr, image, 0, 0);
	mlx_loop(world->canvas->mlx_ptr);
	mlx_delete_image(world->canvas->mlx_ptr, image);
	mlx_terminate(world->canvas->mlx_ptr);
	free_world(world);
	return (0);
}
