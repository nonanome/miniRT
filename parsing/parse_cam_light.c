/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cam_light.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkuhn <kkuhn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:58:46 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/21 20:31:48 by kkuhn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	parse_ambient_light(t_world *world, char *line)
{
	char	**split;
	char	**rgb_split;
	double	ratio;
	double	r;
	double	g;
	double	b;

	check_ambient_light_line(line);
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






int	parse_camera(t_world *world, char *line)
{
	char	**split;
	char	**xyz_split;
	char	**normal_split;
	double	xyz[3];
	double	normal[3];

	check_camera_line(line);
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
	return (ft_free_split(split), ft_free_split(xyz_split), ft_free_split(normal_split), 0);
}

void realloc_light(t_c *canvas, t_light new_light)
{
	t_light *lights;
	int i;

	i = 0;
	lights = malloc((canvas->num_lights + 1) * sizeof(t_light)); 
	while (i < canvas->num_lights)
	{
		printf("test");
		lights[i] = canvas->lightsource[i];
		i ++;
	}
	lights[i] = new_light;
	printf("newlight");
	FREE(canvas->lightsource);
	canvas->lightsource = lights;
	canvas->num_lights ++;
}

int	parse_light(t_world *world, char *line)
{
	char	**split;
	char	**xyz_split;
	char	**rgb_split;
	double	xyz[3];
	double	rgb[3];
	double	brightness;
	t_light new_light;

	check_spere_or_light_line(line);
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
	new_light.color = set_vector(rgb[0], rgb[1], rgb[2],
			brightness);
	new_light.position = set_vector(xyz[0], xyz[1], xyz[2], 0);
	realloc_light(world->canvas, new_light);
	return (ft_free_split(split), ft_free_split(xyz_split),
		ft_free_split(rgb_split), 0);
}
