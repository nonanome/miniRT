/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:33:24 by qhahn             #+#    #+#             */
/*   Updated: 2025/02/05 16:01:31 by qhahn            ###   ########.fr       */
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

typedef struct s_comp
{
	double	t;
	t_sphere	*object;
	xyzvektor	point;
	xyzvektor	eyev;
	xyzvektor	normalv;
	xyzvektor	reflectv;
	bool	inside;
}				t_comp;

typedef struct s_camera
{
	int		hsize;
	int		vsize;
	double	pixel_size;
	double	field_of_view;
	double	**transform;
	double	half_width;
	double	half_height;
}				t_camera;

t_world			*get_world(int spheres);
void			free_world(t_world *world);
int				intersect_world(t_world *world, t_ray ray);
t_comp			prepare_computations(t_intersec *intersection, t_ray ray,
		t_sphere *shape);
xyzvektor 		shade_hit(t_world *world, t_comp comp);
xyzvektor 		color_at(t_world *world, t_ray ray);
double			**view_transform(xyzvektor from, xyzvektor to, xyzvektor up);
t_camera		camera(int hsize, int vsize, double field_of_view);

#endif