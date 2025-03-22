/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shapes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:00:05 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/22 17:18:04 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	parse_common_shape(char **split, double pos[3], double rgb[3])
{
	char	**pos_split;
	char	**rgb_split;

	pos_split = ft_split(split[1], ',');
	if (!pos_split)
		return (1);
	rgb_split = ft_split(split[3], ',');
	if (!rgb_split)
		return (ft_free_split(pos_split), 1);
	if (split[0][0] == 'c')
	{
		ft_free_split(rgb_split);
		rgb_split = ft_split(split[5], ',');
	}
	if (!parse_xyz(pos_split, pos, 0))
		return (ft_free_split(pos_split), ft_free_split(rgb_split), 1);
	if (!parse_xyz(rgb_split, rgb, 0))
		return (ft_free_split(pos_split), ft_free_split(rgb_split), 1);
	rgb[0] /= 255;
	rgb[1] /= 255;
	rgb[2] /= 255;
	if (rgb[0] < 0 || rgb[0] > 1 || rgb[1] < 0 || rgb[1] > 1 || rgb[2] < 0
		|| rgb[2] > 1)
		return (ft_free_split(pos_split), ft_free_split(rgb_split), 1);
	return (ft_free_split(pos_split), ft_free_split(rgb_split), 0);
}

void	add_checker(t_world *world, char **split, int type)
{
	int		i;
	char	***split_split;
	int		j;

	i = 0;
	j = 0;
	while (split[i++])
		;
	if ((i != 8 && (type == 0 || type == 1)) || (i != 10 && type == 2))
		return ;
	split_split = ft_calloc(4, sizeof(char **));
	if (type == 0 || type == 1)
		i = 4;
	else
		i = 6;
	while (split[i])
	{
		split_split[j] = ft_split(split[i], ',');
		i++;
		j++;
	}
	world->shapes[world->nr_shapes
		- 1]->material.checker.color1 = get_color_from_uint(world->shapes[world->nr_shapes
			- 1]->material.color);
	world->shapes[world->nr_shapes
		- 1]->material.checker.color2 = set_vector(budget_ft_atof(split_split[0][0])
			/ 255, budget_ft_atof(split_split[0][1]) / 255,
			budget_ft_atof(split_split[0][2]) / 255, 1);
	world->shapes[world->nr_shapes - 1]->material.checker.enable = true;
	world->shapes[world->nr_shapes
		- 1]->material.checker.origin = set_vector(budget_ft_atof(split_split[1][0]),
			budget_ft_atof(split_split[1][1]),
			budget_ft_atof(split_split[1][2]), 0);
}

int	parse_sphere(t_world *world, char *line)
{
	char	**split;
	double	xyz[3];
	double	rgb[3];
	double	radius;
	t_shape	*shape;

	check_spere_or_light_line(line);
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
	add_checker(world, split, 0);
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

	check_plane_line(line);
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
	add_checker(world, split, 1);
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

	check_cylinder_line(line);
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
	shape->material.color = get_color_from_tuple(set_vector(rgb[0], rgb[1],
				rgb[2], 0));
	free_double_ptr(shape->default_transformation, 4);
	shape->default_transformation = translation(xyz[0], xyz[1], xyz[2]);
	shape->radius = budget_ft_atof(split[3]) / 2;
	shape->maximum = budget_ft_atof(split[4]) / 2;
	shape->minimum = -budget_ft_atof(split[4]) / 2;
	if (split[5])
		shape->closed = budget_ft_atof(split[5]);
	world->shapes[world->nr_shapes] = shape;
	world->nr_shapes++;
	ft_free_split(normal_split);
	add_checker(world, split, 2);
	return (ft_free_split(split), 0);
}
