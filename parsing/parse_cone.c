/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:25:28 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/19 17:53:41 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	prepare_cone_vars(t_shape *shape, t_world *world, char **split,
		double rgb[3])
{
	int	i;

	i = 0;
	shape->material.ambient = world->ambient_intensity;
	shape->material.color = get_color_from_tuple(set_vector(rgb[0], rgb[1],
				rgb[2], 0));
	shape->radius = budget_ft_atof(split[3]) / 2;
	shape->maximum = budget_ft_atof(split[4]);
	shape->minimum = 0;
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

int	parse_cone(t_world *world, char *line)
{
	char	**split;
	char	**normal_split;
	double	xyz[6];
	double	normal[3];
	t_shape	*shape;

	check_cylinder_line(line, world);
	split = ft_split(line, ' ');
	if (!split)
		return (1);
	if (parse_common_shape(split, xyz, &(xyz[3]), world))
		return (ft_free_split(split), 1);
	normal_split = ft_split(split[2], ',');
	if (!normal_split)
		return (ft_free_split(split), 1);
	if (!parse_xyz(normal_split, normal, 1, world))
		return (ft_free_split(normal_split), ft_free_split(split), 1);
	shape = new_shape(3);
	shape->normal = normalize(set_vector(normal[0], normal[1], normal[2], 0));
	prepare_cone_vars(shape, world, split, &(xyz[3]));
	shape->default_transformation = get_cylinder_matrix(xyz, shape->normal,
			shape->radius);
	if (!shape->default_transformation)
		return (1);
	return (ft_free_split(split), 0);
}
