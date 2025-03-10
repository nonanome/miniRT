/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 14:40:27 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/10 18:14:14 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "MLX42/include/MLX42/MLX42.h"
# include "libft/libft.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>

# define PI 3.14159265358979323846
# define EPSILON 0.00001

static int			globalID = 0;

typedef struct s_koord
{
	double			x;
	double			y;
	double			z;
	double			w;
}					xyzvektor;

typedef struct point_data
{
	xyzvektor		position;
	xyzvektor		velocity;
}					t_point;

typedef struct environment
{
	xyzvektor		wind;
	xyzvektor		gravity;
}					t_env;

typedef struct ray
{
	xyzvektor		origin;
	xyzvektor		direction;

}					t_ray;

typedef struct material
{
	uint32_t		color;
	double			ambient;
	double			diffuse;
	double			specular;
	double			shininess;
}					t_material;

typedef struct s_shape
{
	int				id;
	xyzvektor		origin;
	double			**default_transformation;
	t_material		material;
	int				type;
	double			radius;
	xyzvektor		normal;
	double			minimum;
	double			maximum;
	bool			closed;
}					t_shape;

typedef struct sphere
{
	int				id;
	xyzvektor		origin;
	double			radius;
	double			**default_transformation;
	t_material		material;
}					t_sphere;

typedef struct intersect
{
	t_ray			ray;
	double			*times;
	int				object_id;
}					t_intersec;

typedef struct intersections
{
	t_intersec		*intersections;
	size_t			nr_intersections;
	size_t			nr_intersection_entries;
}					t_all_intersec;

typedef struct pointlight
{
	xyzvektor		color;
	xyzvektor		position;
}					t_light;

typedef struct reflection_data_store
{
	xyzvektor		effective_color;
	xyzvektor		materialcolor;
	xyzvektor		lightsourcecolor;
	xyzvektor		light_vector;
	xyzvektor		ambient;
	xyzvektor		diffuse;
	xyzvektor		specular;
	double			reflect_dot_eye;
	double			factor;
	xyzvektor		reflectv;
}					t_store;

typedef struct canvas
{
	int				width;
	int				height;
	double			pixel_size;
	double			half_size;
	double			worldheight;
	mlx_image_t		*img;
	mlx_t			*mlx_ptr;
	t_all_intersec	all_intersections;
	t_light			lightsource;
	xyzvektor		normale;
	xyzvektor		eyevector;
}					t_c;

typedef struct visualize_struct
{
	xyzvektor		world_coordinates;
	xyzvektor		intersectionpoint;
	xyzvektor		color;
	t_intersec		*intersec;
}					v_s;

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

typedef struct s_world
{
	t_shape	**shapes;
	long		nr_shapes;
	t_c			*canvas;
	double		*all_sorted;
	t_env		*env;
	xyzvektor	*ambient;
	double		ambient_intensity;
	t_camera	*camera;
}				t_world;

typedef struct s_comp
{
	double	t;
	t_shape	*object;
	xyzvektor	point;
	xyzvektor	over_point;
	xyzvektor	eyev;
	xyzvektor	normalv;
	xyzvektor	reflectv;
	bool	inside;
}				t_comp;

#endif