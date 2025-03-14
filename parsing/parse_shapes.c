/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shapes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:00:05 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/14 19:55:50 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	parse_sphere(t_world *world, char *line)
{
	char	**split;
	double	xyz[3];
	double	rgb[3];
	double	radius;
	t_shape	*shape;

	split = ft_split(line, ' ');
	if (!split)
		return (1);
	if (parse_common_shape(split, xyz, rgb))
		return (ft_free_split(split), 1);
	radius = budget_ft_atof(split[2]);
	if (radius < 0)
		return (ft_free_split(split), 1);
	shape = new_shape(0);
	shape->origin = set_vector(xyz[0], xyz[1], xyz[2], 1);
	shape->material.ambient = world->ambient_intensity;
	shape->material.color = get_color_from_tuple(set_vector(rgb[0], rgb[1],
				rgb[2], 0));
	shape->radius = radius;
	world->shapes[world->nr_shapes] = shape;
	world->nr_shapes++;
	return (ft_free_split(split), 0);
}

int	parse_plane(t_world *world, char *line)
{
	char	**split;
	char	**normal_split;
	double	xyz[3];
	double	rgb[3];
	double	normal[3];
	t_shape	*shape;

	split = ft_split(line, ' ');
	if (!split)
		return (1);
	if (parse_common_shape(split, xyz, rgb))
		return (ft_free_split(split), 1);
	normal_split = ft_split(split[2], ',');
	if (!normal_split)
		return (ft_free_split(split), 1);
	if (!parse_xyz(normal_split, normal, 1) || normal[0] < -1 || normal[0] > 1
		|| normal[1] < -1 || normal[1] > 1 || normal[2] < -1 || normal[2] > 1)
		return (ft_free_split(normal_split), ft_free_split(split), 1);
	shape = new_shape(1);
	shape->normal = set_vector(normal[0], normal[1], normal[2], 0);
	shape->material.ambient = world->ambient_intensity;
	shape->material.color = get_color_from_tuple(set_vector(rgb[0], rgb[1],
				rgb[2], 0));
	free_double_ptr(shape->default_transformation, 4);
	shape->default_transformation = translation(xyz[0], xyz[1], xyz[2]);
	world->shapes[world->nr_shapes] = shape;
	world->nr_shapes++;
	ft_free_split(normal_split);
	return (ft_free_split(split), 0);
}

int	parse_cylinder(t_world *world, char *line)
{
	char	**split;
	char	**normal_split;
	double	xyz[3];
	double	rgb[3];
	double	normal[3];
	t_shape	*shape;

	split = ft_split(line, ' ');
	if (!split)
		return (1);
	if (parse_common_shape(split, xyz, rgb))
		return (ft_free_split(split), 1);
	normal_split = ft_split(split[2], ',');
	if (!normal_split)
		return (ft_free_split(split), 1);
	if (!parse_xyz(normal_split, normal, 1) || normal[0] < -1 || normal[0] > 1
		|| normal[1] < -1 || normal[1] > 1 || normal[2] < -1 || normal[2] > 1)
		return (ft_free_split(normal_split), ft_free_split(split), 1);
	shape = new_shape(2);
	shape->normal = set_vector(normal[0], normal[1], normal[2], 0);
	shape->material.ambient = world->ambient_intensity;
	shape->material.color = get_color_from_tuple(set_vector(rgb[0], rgb[1], rgb[2], 0));
	free_double_ptr(shape->default_transformation, 4);
	shape->default_transformation = translation(xyz[0], xyz[1], xyz[2]);
	shape->radius = budget_ft_atof(split[3]) / 2;
	shape->maximum = budget_ft_atof(split[4]);
	if (split[5])
		shape->closed = budget_ft_atof(split[5]);
	world->shapes[world->nr_shapes] = shape;
	world->nr_shapes++;
	ft_free_split(normal_split);
	return (ft_free_split(split), 0);
}