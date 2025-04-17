/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:00:05 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/17 17:51:10 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

double	**get_sphere_matrix(double radius, double xyz[3])
{
	double	**trans;
	double	**rad;

	trans = translation(xyz[0], xyz[1], xyz[2]);
	rad = scaling(radius, radius, radius);
	return (multiply_matrix(rad, trans));
}

int	parse_sphere(t_world *world, char *line)
{
	char	**split;
	double	xyz[3];
	double	rgb[3];
	double	radius;
	t_shape	*shape;

	check_sphere_line(line, world);
	split = ft_split(line, ' ');
	if (!split)
		return (1);
	if (parse_common_shape(split, xyz, rgb))
		return (ft_free_split(split), 1);
	radius = budget_ft_atof(split[2]);
	if (radius < 0)
		return (ft_free_split(split), 1);
	shape = new_shape(0);
	shape->default_transformation = get_sphere_matrix(radius, xyz);
	shape->material.ambient = world->ambient_intensity;
	shape->material.color = get_color_from_tuple(set_vector(rgb[0], rgb[1],
				rgb[2], 0));
	shape->radius = radius;
	world->shapes[world->nr_shapes] = shape;
	world->nr_shapes++;
	add_checker(world, split, 0);
	return (ft_free_split(split), 0);
}

int	parse_plane(t_world *world, char *line)
{
	char	**split;
	double	xyz[3];
	double	rgb[3];
	double	normal[3];
	t_shape	*shape;

	split = ft_split(line, ' ');
	if (!split)
		return (1);
	if (!split || parse_common_shape(split, xyz, rgb)
		|| parse_normal_vector(split, normal) || check_plane_line(line))
		return (1);
	shape = new_shape(1);
	shape->normal = set_vector(normal[0], normal[1], normal[2], 0);
	shape->material.ambient = world->ambient_intensity;
	shape->material.color = get_color_from_tuple(set_vector(rgb[0], rgb[1],
				rgb[2], 0));
	shape->default_transformation = translation(xyz[0], xyz[1], xyz[2]);
	world->shapes[world->nr_shapes] = shape;
	world->nr_shapes++;
	add_checker(world, split, 1);
	return (ft_free_split(split), 0);
}
