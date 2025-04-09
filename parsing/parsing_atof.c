/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_atof.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:03:14 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/09 22:47:06 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

double	budget_ft_atof(char *str)
{
	double	result;
	double	fraction;
	int		sign;
	int		i;
	int		frac_digits;

	i = 0;
	result = 0;
	fraction = 0;
	sign = 1;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] && str[i] == '.')
	{
		i++;
		frac_digits = 0;
		while (ft_isdigit(str[i]))
		{
			fraction = fraction * 10 + (str[i] - '0');
			frac_digits++;
			i++;
		}
		while (frac_digits-- > 0)
			fraction /= 10;
	}
	return ((result + fraction) * sign);
}
