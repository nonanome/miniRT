/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkuhn <kkuhn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:33:24 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/22 15:21:25 by kkuhn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_H
# define WORLD_H

# include "../structs.h"
# include "../miniRT.h"

double		**multiply_matrix(double **a, double **b);
xyzvektor	calculate_normale(t_shape shape, xyzvektor point);
xyzvektor	negateTuple(xyzvektor a);
xyzvektor	point_of_intersection(t_intersec *intersec, t_ray ray);
xyzvektor	set_black(void);
xyzvektor	crossProduct(xyzvektor a, xyzvektor b);
double		**get_identity_matrix(void);
xyzvektor	substraction(xyzvektor a, xyzvektor b);
xyzvektor	normalize(xyzvektor a);
xyzvektor	multiply_vector_and_matrix(xyzvektor a, double **b);
double		**invert_matrix(double **a, int size);
xyzvektor	set_vector(double x, double y, double z, double w);
double		**translation(double x, double y, double z);
t_material	default_material(void);
void		init_canvas(t_c *canvas);
double		**scaling(double x, double y, double z);
double		**rotation_x(double degree);
double		**rotation_y(double degree);
double		**rotation_z(double degree);
uint32_t	get_color_from_tuple(xyzvektor color);

t_world		*get_world(int shapes);
void		free_world(t_world *world);
int			intersect_world(t_world *world, t_ray ray);
t_intersec	*intersect(t_shape *shape, t_ray ray);
t_comp		prepare_computations(t_intersec *intersection, t_ray ray,
				t_shape *shape);
xyzvektor	shade_hit(t_world *world, t_comp comp);
xyzvektor	color_at(t_world *world, t_ray ray);
double		**view_transform(xyzvektor from, xyzvektor to, xyzvektor up);
t_camera	*camera(int hsize, int vsize, double field_of_view);
t_ray		ray_for_pixel(t_camera *cam, int px, int py);
mlx_image_t	*render_image(t_camera *cam, t_world *world);
void		find_nearest_intersection(t_intersec *intersections,
				int nr_intersections, t_intersec **intersec_to_use,
				int *shape_to_use);
t_shape		*new_shape(int type);
double		dotProduct(xyzvektor a, xyzvektor b);
xyzvektor	addition(xyzvektor a, xyzvektor b);
xyzvektor	scalarMultiplication(xyzvektor a, double b);
xyzvektor	lightning(t_material material, xyzvektor point, t_c canvas,
				bool *in_shadow);
t_ray		init_ray(void);
double		magnitude(xyzvektor a);
int			parse_input(char *file_name, t_world *world);
void		free_double_ptr(double **a, int size);
void		empty_intersections(t_c *canvas);
// test
bool		*is_shadowed(t_world *world, xyzvektor point);

#endif