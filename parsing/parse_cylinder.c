/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:15:38 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/18 19:28:22 by qhahn            ###   ########.fr       */
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
	shape->closed = true;
	shape->minimum = -budget_ft_atof(split[4]) / 2;
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
		return (NULL);
	scale = scaling(radius, 1.0, radius);
	if (!scale)
		return (NULL);
	temp_transform = multiply_matrix(translation_matrix, rotation);
	if (!temp_transform)
		return (NULL);
	full_transform = multiply_matrix(temp_transform, scale);
	if (!full_transform)
		return (NULL);
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
	if (!parse_xyz(normal_split, normal, 1))
		return (ft_free_split(normal_split), ft_free_split(split), 1);
	shape = new_shape(2);
	shape->normal = normalize(set_vector(normal[0], normal[1], normal[2], 0));
	prepare_cylinder_vars(shape, world, split, &(xyz[3]));
	shape->default_transformation = get_cylinder_matrix(xyz, shape->normal,
			shape->radius);
	if (!shape->default_transformation)
		return (1);
	return (ft_free_split(split), 0);
}
