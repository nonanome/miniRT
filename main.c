/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:20:56 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/18 13:22:43 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world_building/world.h"

#define PI 3.14159265358979323846

void	esc_handler(mlx_key_data_t keydata, void *param)
{
	t_world	*world;

	if (keydata.key == MLX_KEY_ESCAPE)
	{
		world = (t_world *)param;
		mlx_delete_image(world->canvas->mlx_ptr, world->canvas->image_to_free);
		mlx_terminate(world->canvas->mlx_ptr);
		exit(0);
	}
}

int	main(int argc, char *argv[])
{
	t_world		*world;
	t_ray		ray;
	t_comp		comp;
	t_xyzvektor	shadestuff;
	mlx_image_t	*image;

	world = get_world(100);
	if (!world || argc != 2)
		return (write(2, "Error\n", 6), 1);
	init_canvas(world->canvas);
	mlx_key_hook(world->canvas->mlx_ptr, esc_handler, world);
	if (parse_input(argv[1], world))
		return (1);
	image = render_image(world->camera, world);
	if (!image)
		return (write(2, "Error\nNo image\n", 15), 1);
	world->canvas->image_to_free = image;
	mlx_image_to_window(world->canvas->mlx_ptr, image, 0, 0);
	mlx_loop(world->canvas->mlx_ptr);
	mlx_delete_image(world->canvas->mlx_ptr, image);
	mlx_terminate(world->canvas->mlx_ptr);
	return (0);
}
