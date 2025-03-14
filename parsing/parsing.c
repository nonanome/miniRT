/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:54:58 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/14 15:46:15 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	**process_file(char *file_name)
{
	char	**input;
	int		fd;
	char	*line;
	int		i;

	i = 0;
	fd = open(file_name, O_RDONLY);
	input = malloc(100 * sizeof(char *));
	if (!input)
		return (NULL);
	while (budget_get_next_line(fd, &line) > 0)
	{
		input[i] = line;
		i++;
	}
	input[i] = line;
	input[++i] = NULL;
	return (input);
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	parse_ambient_light(t_world *world, char *line)
{
	char	**split;
	char	**rgb_split;
	double	ratio;
	double	r;
	double	g;
	double	b;

	split = ft_split(line, ' ');
	if (!split)
		return (1);
	ratio = budget_ft_atof(split[1]);
	if (ratio < 0 || ratio > 1)
		return (ft_free_split(split), 1);
	rgb_split = ft_split(split[2], ',');
	if (!rgb_split)
		return (ft_free_split(split), 1);
	r = ft_atoi(rgb_split[0]) / 255.0;
	g = ft_atoi(rgb_split[1]) / 255.0;
	b = ft_atoi(rgb_split[2]) / 255.0;
	if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1)
		return (ft_free_split(split), ft_free_split(rgb_split), 1);
	*(world->ambient) = set_vector(r, g, b, 0);
	world->ambient_intensity = ratio;
	ft_free_split(rgb_split);
	ft_free_split(split);
	return (0);
}

int	parse_xyz(char **split, double *xyz, int mode)
{
	int	i;

	i = 0;
	while (split[i])
	{
		xyz[i] = budget_ft_atof(split[i]);
		if (mode == 1 && (xyz[i] < -1 || xyz[i] > 1))
			return (0);
		i++;
	}
	return (1);
}

int	parse_camera(t_world *world, char *line)
{
	char	**split;
	char	**xyz_split;
	char	**normal_split;
	double	xyz[3];
	double	normal[3];

	split = ft_split(line, ' ');
	if (!split)
		return (1);
	if (budget_ft_atof(split[2]) < 0 || budget_ft_atof(split[2]) > 180)
		return (ft_free_split(split), 1);
	xyz_split = ft_split(split[1], ',');
	if (!xyz_split)
		return (ft_free_split(split), 1);
	if (!parse_xyz(xyz_split, xyz, 0))
		return (ft_free_split(split), ft_free_split(xyz_split), 1);
	normal_split = ft_split(split[2], ',');
	if (!normal_split)
		return (ft_free_split(split), ft_free_split(xyz_split), 1);
	if (!parse_xyz(normal_split, normal, 1))
		return (ft_free_split(split), ft_free_split(xyz_split),
			ft_free_split(normal_split), 1);
	if (normal[0] < -1 || normal[0] > 1 || normal[1] < -1 || normal[1] > 1
		|| normal[2] < -1 || normal[2] > 1)
		return (ft_free_split(split), ft_free_split(xyz_split),
			ft_free_split(normal_split), 1);
	world->camera->field_of_view = budget_ft_atof(split[3]);
	world->camera->transform = view_transform(set_vector(xyz[0], xyz[1], xyz[2],
				1), set_vector(normal[0], normal[1], normal[2], 0),
			set_vector(0, 1, 0, 0));
	return (ft_free_split(split), ft_free_split(xyz_split), 0);
}

int	parse_light(t_world *world, char *line)
{
	char	**split;
	char	**xyz_split;
	char	**rgb_split;
	double	xyz[3];
	double	rgb[3];
	double	brightness;

	split = ft_split(line, ' ');
	if (!split)
		return (1);
	if (budget_ft_atof(split[2]) < 0)
		return (ft_free_split(split), 1);
	xyz_split = ft_split(split[1], ',');
	if (!xyz_split)
		return (ft_free_split(split), 1);
	rgb_split = ft_split(split[3], ',');
	if (!rgb_split)
		return (ft_free_split(split), ft_free_split(xyz_split), 1);
	if (!parse_xyz(xyz_split, xyz, 0))
		return (ft_free_split(split), ft_free_split(xyz_split),
			ft_free_split(rgb_split), 1);
	if (!parse_xyz(rgb_split, rgb, 0))
		return (ft_free_split(split), ft_free_split(xyz_split),
			ft_free_split(rgb_split), 1);
	rgb[0] /= 255;
	rgb[1] /= 255;
	rgb[2] /= 255;
	if (rgb[0] < 0 || rgb[0] > 1 || rgb[1] < 0 || rgb[1] > 1 || rgb[2] < 0
		|| rgb[2] > 1)
		return (ft_free_split(split), ft_free_split(xyz_split),
			ft_free_split(rgb_split), 1);
	brightness = budget_ft_atof(split[2]);
	if (brightness < 0 || brightness > 1)
		return (ft_free_split(split), ft_free_split(xyz_split),
			ft_free_split(rgb_split), 1);
	world->canvas->lightsource.color = set_vector(rgb[0], rgb[1], rgb[2],
			brightness);
	world->canvas->lightsource.position = set_vector(xyz[0], xyz[1], xyz[2], 0);
	return (ft_free_split(split), ft_free_split(xyz_split),
		ft_free_split(rgb_split), 0);
}

int	parse_common_shape(char **split, double pos[3], double rgb[3])
{
	char	**pos_split;
	char	**rgb_split;

	pos_split = ft_split(split[1], ',');
	if (!pos_split)
		return (1);
	rgb_split = ft_split(split[3], ',');
	if (split[0][0] == 'c')
		rgb_split = ft_split(split[5], ',');
	if (!rgb_split)
		return (ft_free_split(pos_split), 1);
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

int	parse_input(char *file_name, t_world *world)
{
	char	**input;
	int		i;

	i = 0;
	input = process_file(file_name);
	while (input[i])
	{
		if (input[i][0] == 'A')
		{
			if (parse_ambient_light(world, input[i]))
				return (1);
		}
		else if (input[i][0] == 'C')
		{
			if (parse_camera(world, input[i]))
				return (1);
		}
		else if (input[i][0] == 'L')
		{
			if (parse_light(world, input[i]))
				return (1);
		}
		else if (ft_strncmp(input[i], "sp", 2) == 0)
		{
			if (parse_sphere(world, input[i]))
				return (1);
		}
		else if (ft_strncmp(input[i], "pl", 2) == 0)
		{
			if (parse_plane(world, input[i]))
				return (1);
		}
		else if (ft_strncmp(input[i], "cy", 2) == 0)
		{
			if (parse_cylinder(world, input[i]))
				return (1);
		}
		i++;
	}
	return (0);
}
