/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:18:39 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/24 16:27:51 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_checker	checker_init(t_xyzvektor color1, t_xyzvektor color2)
{
	t_checker	new;

	new.enable = true;
	new.color1 = color1;
	new.color2 = color2;
	new.origin = set_vector(0, 0, 0, 1);
	new.default_transformation = get_identity_matrix();
	return (new);
}

t_xyzvektor	pattern_at(t_shape shape, t_xyzvektor point)
{
	t_xyzvektor	object_point_vec;
	double		**object_point;
	double		sum;

	object_point = invert_matrix(shape.default_transformation, 4);
	object_point_vec = multiply_vector_and_matrix(point, object_point);
	sum = floor(point.x) + floor(point.y) + floor(point.z);
	if (fmod(sum, 2.0) == 0.0)
		return (shape.material.checker.color1);
	else
		return (shape.material.checker.color2);
}
