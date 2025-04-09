/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_cone.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:15:38 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/10 01:48:22 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	prepare_cylinder_vars(t_shape *shape, t_world *world, char **split,
		double rgb[3])
{
	int	i;

	i = 0;
	shape->material.ambient = world->ambient_intensity;
	shape->material.color = get_color_from_tuple(set_vector(rgb[0], rgb[1],
				rgb[2], 0));
	shape->radius = budget_ft_atof(split[3]) / 2;
	shape->maximum = budget_ft_atof(split[4]) / 2;
	shape->minimum = -budget_ft_atof(split[4]) / 2;
	while (split[i++])
		;
	if (i == 8 || i == 9)
		shape->closed = budget_ft_atof(split[i - 2]);
	else
		shape->closed = 0;
	world->shapes[world->nr_shapes] = shape;
	world->nr_shapes++;
	add_checker(world, split, 2);
}

double	**get_cylinder_matrix(double xyz[3], t_xyzvektor target_axis,
		double radius)
{
	double	**rotation;
	double	**translation_matrix;
	double	**scale;
	double	**full_transform;
	double	**temp_transform;

	rotation = get_identity_matrix();
	if (!rotation)
		return (NULL);
	create_rotation_matrix(target_axis, rotation);
	translation_matrix = translation(xyz[0], xyz[1], xyz[2]);
	if (!translation_matrix)
	{
		free_double_ptr(rotation, 4);
		return (NULL);
	}
	scale = scaling(radius, 1.0, radius);
	if (!scale)
	{
		free_double_ptr(rotation, 4);
		free_double_ptr(translation_matrix, 4);
		return (NULL);
	}
	temp_transform = multiply_matrix(translation_matrix, rotation);
	if (!temp_transform)
	{
		free_double_ptr(rotation, 4);
		free_double_ptr(translation_matrix, 4);
		free_double_ptr(scale, 4);
		return (NULL);
	}
	full_transform = multiply_matrix(temp_transform, scale);
	if (!full_transform)
	{
		free_double_ptr(rotation, 4);
		free_double_ptr(translation_matrix, 4);
		free_double_ptr(scale, 4);
		free_double_ptr(temp_transform, 4);
		return (NULL);
	}
	free_double_ptr(rotation, 4);
	free_double_ptr(translation_matrix, 4);
	free_double_ptr(scale, 4);
	free_double_ptr(temp_transform, 4);
	return (full_transform);
}

int	parse_cylinder(t_world *world, char *line)
{
	char	**split;
	char	**normal_split;
	double	xyz[6];
	t_shape	*shape;
	double	normal[3];

	check_cylinder_line(line);
	split = ft_split(line, ' ');
	if (!split)
		return (1);
	if (parse_common_shape(split, xyz, &(xyz[3])))
		return (ft_free_split(split), 1);
	normal_split = ft_split(split[2], ',');
	if (!normal_split)
		return (ft_free_split(split), 1);
	if (!parse_xyz(normal_split, normal, 1) || normal[0] < -1 || normal[0] > 1
		|| normal[1] < -1 || normal[1] > 1 || normal[2] < -1 || normal[2] > 1)
		return (ft_free_split(normal_split), ft_free_split(split), 1);
	shape = new_shape(2);
	shape->normal = set_vector(normal[0], normal[1], normal[2], 0);
	ft_free_split(normal_split);
	prepare_cylinder_vars(shape, world, split, &(xyz[3]));
	shape->default_transformation = get_cylinder_matrix(xyz, shape->normal,
			shape->radius);
	if (!shape->default_transformation)
	{
		ft_free_split(split);
		free(shape);
		return (1);
	}
	shape->inverse = invert_matrix(shape->default_transformation, 4);
	if (!shape->inverse)
	{
		free_double_ptr(shape->default_transformation, 4);
		ft_free_split(split);
		free(shape);
		return (1);
	}
	return (ft_free_split(split), 0);
}

int	parse_cone(t_world *world, char *line)
{
	char	**split;
	char	**normal_split;
	double	xyz[6];
	double	normal[3];
	t_shape	*shape;

	check_cylinder_line(line);
	split = ft_split(line, ' ');
	if (!split)
		return (1);
	if (parse_common_shape(split, xyz, &(xyz[3])))
		return (ft_free_split(split), 1);
	normal_split = ft_split(split[2], ',');
	if (!normal_split)
		return (ft_free_split(split), 1);
	if (!parse_xyz(normal_split, normal, 1) || normal[0] < -1 || normal[0] > 1
		|| normal[1] < -1 || normal[1] > 1 || normal[2] < -1 || normal[2] > 1)
		return (ft_free_split(normal_split), ft_free_split(split), 1);
	shape = new_shape(3);
	shape->normal = set_vector(normal[0], normal[1], normal[2], 0);
	ft_free_split(normal_split);
	prepare_cylinder_vars(shape, world, split, &(xyz[3]));
	shape->default_transformation = get_cylinder_matrix(xyz, shape->normal,
			shape->radius);
	if (!shape->default_transformation)
	{
		ft_free_split(split);
		free(shape);
		return (1);
	}
	shape->inverse = invert_matrix(shape->default_transformation, 4);
	if (!shape->inverse)
	{
		free_double_ptr(shape->default_transformation, 4);
		ft_free_split(split);
		free(shape);
		return (1);
	}
	return (ft_free_split(split), 0);
}
