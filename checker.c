/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:18:39 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/07 19:18:09 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_xyzvektor	pattern_at(t_shape shape, t_xyzvektor point)
{
	double		**object_point;
	double		sum;

	sum = floor(point.x) + floor(point.y) + floor(point.z);
	if (fmod(sum, 2.0) == 0.0)
		return (get_color_from_uint(shape.material.color));
	else
		return (shape.material.color2);
}
