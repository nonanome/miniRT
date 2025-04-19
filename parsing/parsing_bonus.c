/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:54:58 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/19 15:35:03 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	look_for_double_uppercase(char **input, int A, int C, int L)
{
	while (*input)
	{
		if (*input[0] == 'A')
			A++;
		if (*input[0] == 'C')
			C++;
		if (*input[0] == 'L')
			L++;
		else if (*input[0] >= '0' && *input[0] <= '9')
		{
			bail("first characters needs to be char", 1);
		}
		input++;
	}
	if (A > 1 || C > 1)
	{
		write(2, "input contains double capital letter object",
			ft_strlen("input contains double capital letter object"));
		exit(0);
	}
}

char	**process_file(char *file_name)
{
	char	**input;
	int		fd;
	char	*line;
	int		i;

	i = 0;
	if (ft_strncmp(&(file_name[ft_strlen(file_name) - 4]), ".rt", 4) != 0)
		bail("only .rt files", 1);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		bail("file not found", 1);
	input = MALLOC(1000 * sizeof(char *));
	if (!input)
		return (NULL);
	while (budget_get_next_line(fd, &line) > 0)
	{
		input[i] = line;
		i++;
	}
	input[i] = line;
	input[++i] = NULL;
	look_for_double_uppercase(input, 0, 0, 0);
	close(fd);
	return (input);
}

static void	free_input(char **input)
{
	int	i;

	i = 0;
	if (!input)
		return ;
	while (input[i])
		FREE(input[i++]);
	FREE(input);
}

static int	parse_line(t_world *world, char *line)
{
	if (line[0] == 'A')
		return (parse_ambient_light(world, line));
	if (line[0] == 'C')
		return (parse_camera(world, line));
	if (line[0] == 'L')
		return (parse_light(world, line));
	if (!ft_strncmp(line, "sp", 2))
		return (parse_sphere(world, line));
	if (!ft_strncmp(line, "pl", 2))
		return (parse_plane(world, line));
	if (!ft_strncmp(line, "cy", 2))
		return (parse_cylinder(world, line));
	return (0);
}

int	parse_input(char *file_name, t_world *world)
{
	char	**input;
	char	*cleaned;
	int		i;

	i = 0;
	world->all_sorted = ft_calloc(sizeof(double *), 100);
	if (!world->all_sorted)
		return (1);
	world->all_sorted[0] = 0;
	input = process_file(file_name);
	if (!input)
		return (1);
	while (input[i])
	{
		cleaned = clean_input_line(input[i]);
		if ((parse_line(world, cleaned)))
			return (1);
		else if (ft_strncmp(cleaned, "co", 2) == 0)
		{
			if (parse_cone(world, cleaned))
				return (1);
		}
		i++;
	}
	return (free_input(input), 0);
}
