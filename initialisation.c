/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:29:16 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/18 19:15:44 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	init_canvas(t_c *canvas)
{
	canvas->worldheight = 8;
	canvas->height = 200;
	canvas->width = 200;
	canvas->mlx_ptr = mlx_init(canvas->width, canvas->height, "miniRT", false);
	canvas->pixel_size = canvas->height / canvas->worldheight;
	canvas->half_size = canvas->worldheight / 2;
	canvas->all_intersections.nr_intersections = 0;
	canvas->all_intersections.intersections = NULL;
	canvas->num_lights = 0;
}

t_ray	init_ray(void)
{
	t_ray		ray;
	t_xyzvektor	origin;
	t_xyzvektor	direction;

	origin.x = 0;
	origin.y = 0;
	origin.z = -5;
	origin.w = 1;
	direction.x = 0;
	direction.y = 0;
	direction.z = 1;
	direction.w = 0;
	ray.origin = origin;
	ray.direction = direction;
	return (ray);
}

void	bail(char *str, int code)
{
	write(2, "Error\n", 6);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	exit(code);
}
