/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   throw_shade.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:14:50 by qhahn             #+#    #+#             */
/*   Updated: 2025/02/03 17:30:16 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

xyzvektor shade_hit(t_world *world, t_comp comp)
{
	xyzvektor retvalue;
	t_c local_canvas;

	local_canvas = *(world->canvas);
	local_canvas.normale = comp.normalv;
	local_canvas.eyevector = comp.eyev;
	return (lightning(comp.object->material, comp.point, local_canvas));
}
