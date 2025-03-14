/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:03:18 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/14 17:03:30 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
