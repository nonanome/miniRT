/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:33:24 by qhahn             #+#    #+#             */
/*   Updated: 2025/02/03 16:07:12 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_H
# define WORLD_H

# include "../miniRT.h"

typedef struct s_world
{
	t_sphere	*spheres;
	long		nr_spheres;
	t_c			*canvas;
	double		*all_sorted;
	t_env		*env;
}				t_world;

t_world			*get_world(int spheres);
void			free_world(t_world *world);
int				intersect_world(t_world *world, t_ray ray);
#endif