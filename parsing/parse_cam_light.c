/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cam_light.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:58:46 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/22 17:16:03 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	parse_xyz_cam_light(char *str, double xyz[3], int is_normal)
{
	char	**split;
	int		i;

	i = -1;
	split = ft_split(str, ',');
	if (!split || count_split(split) != 3)
	{
		ft_free_split(split);
		return (1);
	}
	while (++i < 3)
	{
		xyz[i] = budget_ft_atof(split[i]);
		if (is_normal && (xyz[i] < -1.0 || xyz[i] > 1.0))
		{
			ft_free_split(split);
			return (1);
		}
	}
	ft_free_split(split);
	return (0);
}

int	parse_fov(char *str, double *fov)
{
	*fov = budget_ft_atof(str);
	if (*fov < 0.0 || *fov > 180.0)
		return (1);
	return (0);
}

int	parse_ambient_light(t_world *world, char *line)
{
	char	**split;
	int		error;
	double	ratio;
	double	rgb[3];

	check_ambient_light_line(line);
	split = ft_split(line, ' ');
	if (!split)
		return (1);
	error = parse_ratio(split[1], &ratio) || parse_rgb(split[2], rgb);
	if (!error)
	{
		*(world->ambient) = set_vector(rgb[0], rgb[1], rgb[2], 0);
		world->ambient_intensity = ratio;
	}
	ft_free_split(split);
	return (error);
}

int	parse_camera(t_world *world, char *line)
{
	char	**split;
	int		error;
	double	xyz[3];
	double	fov;
	double	normal[3];

	check_camera_line(line);
	split = ft_split(line, ' ');
	if (!split)
		return (1);
	error = parse_xyz_cam_light(split[1], xyz, 0);
	if (!error)
		error = parse_xyz_cam_light(split[2], normal, 1);
	if (!error)
		error = parse_fov(split[3], &fov);
	if (!error)
	{
		world->camera->field_of_view = fov;
		world->camera->transform = view_transform(set_vector(xyz[0], xyz[1],
					xyz[2], 1), set_vector(normal[0], normal[1], normal[2], 0),
				set_vector(0, 1, 0, 0));
	}
	ft_free_split(split);
	return (error);
}

int	parse_light(t_world *world, char *line)
{
	char	**split;
	double	ratio;
	double	xyz[3];
	double	rgb[3];
	int		error;

	check_spere_or_light_line(line);
	split = ft_split(line, ' ');
	if (!split)
		return (1);
	error = parse_xyz_cam_light(split[1], xyz, 0) || parse_ratio(split[2],
			&ratio) || parse_rgb(split[3], rgb);
	if (!error)
	{
		world->canvas->lightsource.color = set_vector(rgb[0], rgb[1], rgb[2],
				ratio);
		world->canvas->lightsource.position = set_vector(xyz[0], xyz[1], xyz[2],
				0);
	}
	ft_free_split(split);
	return (error);
}
