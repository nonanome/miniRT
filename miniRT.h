#ifndef MINTRT_H

# define MINIRT_H

# include "MLX42/include/MLX42/MLX42.h"
# include "libft/libft.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include "structs.h"

// intersection
void				save_intersection(t_c *canvas, t_intersec *new_intersection,
						int i, int j);
t_intersec			*intersect(t_shape *shape, t_ray ray);
xyzvektor			ray_position(t_ray ray, double time);
double				get_smallest_positive_value(t_intersec *intersection1);
xyzvektor			identify_hit(t_all_intersec all_intersections);
t_ray				transform(t_ray ray, double **transformation);

// trasnformation
double				**translation(double x, double y, double z);
double				**scaling(double x, double y, double z);
double				**rotation_x(double degree);
double				**rotation_y(double degree);
double				**rotation_z(double degree);
double				**shearing(double *proportions);

// vector operations
xyzvektor			crossProduct(xyzvektor a, xyzvektor b);
double				dotProduct(xyzvektor a, xyzvektor b);
xyzvektor			substraction(xyzvektor a, xyzvektor b);
xyzvektor			addition(xyzvektor a, xyzvektor b);
double				magnitude(xyzvektor a);
xyzvektor			normalize(xyzvektor a);
xyzvektor			negateTuple(xyzvektor a);
xyzvektor			scalarMultiplication(xyzvektor a, double b);
xyzvektor			scalarDivision(xyzvektor a, double b);
xyzvektor			hadamard_product(xyzvektor a, xyzvektor b);
xyzvektor			set_vector(double x, double y, double z, double w);

// matrix operations
double				**multiply_matrix(double **a, double **b);
xyzvektor			multiply_vector_and_matrix(xyzvektor a, double **b);
double				**get_identity_matrix(void);
int					compare_matrix(double **a, double **b, int size);
double				**transpose_matrix(double **a, int size);
double				get_minor(double **a, int i, int j, int size);
double				get_determinante_of_2(double **a);
double				*extract_submatrix_row(double *a, int ii, int size);
double				**get_submatrix(double **a, int ii, int jj, int size);
double				**invert_matrix(double **a, int size);
double				get_cofactor(double **a, int i, int j, int size);
double				get_determinant_of_bigger_matrix(double **a, int size);
double				get_determinante_of_3x3(double **a);

// color operations
uint32_t			get_color_from_tuple(xyzvektor color);
void				init_canvas(t_c *canvas);
t_ray				init_ray(void);

// utils
t_light				default_light(void);
t_material			default_material(void);
void				free_double_ptr(double **a, int size);
void				show_matrix(double **a, int size);
void				*MALLOC(int size);
// sphere
void				set_transform(t_shape *shape, double **translation);
t_material			default_material(void);

// ray
xyzvektor			calculate_reflection(xyzvektor in, xyzvektor normale);
xyzvektor			calculate_normale(t_shape shape,
						xyzvektor point);

void				visualize(void *input);
int					min_of(int a, int b);
uint32_t			get_color_from_tuple(xyzvektor color);
xyzvektor			get_color_from_uint(uint32_t color);
xyzvektor			point_of_intersection(t_intersec *intersec, t_ray ray);

xyzvektor			set_black(void);
t_sphere			new_sphere(void);
t_light				default_light(void);
t_material			default_material(void);

xyzvektor			lightning(t_material material, xyzvektor point, t_c canvas, bool in_shadow);
xyzvektor			calculate_reflection(xyzvektor in, xyzvektor normale);
t_shape 			*new_shape(int type);
xyzvektor 			calculate_wall_coordinate(int x, int y, double pixel_size, double half);

#endif