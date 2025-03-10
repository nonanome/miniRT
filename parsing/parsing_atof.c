/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_atof.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:03:14 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/10 18:07:09 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

double budget_ft_atof(char *str)
{
    double result;
    double fraction;
    int sign;
    int i;

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
        result = result * 10 + str[i] - '0';
        i++;
    }
    if (str[i] && str[i] == '.')
    {
        i++;
        while (str[i])
        {
            fraction = fraction * 10 + str[i] - '0';
            i++;
        }
    }
    return (result + fraction / 10);
}
