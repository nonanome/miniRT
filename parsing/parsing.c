/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:54:58 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/14 19:52:53 by qhahn            ###   ########.fr       */
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
	i = 0;
	while (input[i])
		free(input[i++]);
	free(input);
	return (0);
}
