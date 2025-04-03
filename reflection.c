/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:43:31 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/03 21:05:02 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_xyzvektor	calculate_reflection(t_xyzvektor in, t_xyzvektor normale)
{
	double		dot;
	t_xyzvektor	n;

	dot = 2 * dot_product(in, normale);
	n = scalar_multiplication(normale, dot);
	return (normalize(substraction(in, n)));
}

t_xyzvektor	sphere_normal(t_shape shape, t_xyzvektor point)
{
	t_xyzvektor	local_point;
	t_xyzvektor	local_normal;
	t_xyzvektor	world_normal;
	double		**inverse_transform;
	double		**transpose_inverse_transform;

	inverse_transform = invert_matrix(shape.default_transformation, 4);
	local_point = multiply_vector_and_matrix(point, inverse_transform);
	local_normal = local_point;
	transpose_inverse_transform = transpose_matrix(inverse_transform, 4);
	world_normal = multiply_vector_and_matrix(local_normal,
			transpose_inverse_transform);
	world_normal.w = 0;
	free_double_ptr(transpose_inverse_transform, 4);
	free_double_ptr(inverse_transform, 4);
	return (normalize(world_normal));
}

t_xyzvektor	calc_cone_normal(t_shape cone, t_xyzvektor point)
{
	t_xyzvektor	normal;
	double		tangens_theta;

	tangens_theta = cone.radius / (cone.maximum - cone.minimum);
	normal.x = -point.x * tangens_theta * tangens_theta;
	normal.y = point.y;
	normal.z = point.z;
	normal = normalize(normal);
	return (normal);
}

t_xyzvektor	calc_cylinder_normal(t_shape shape, t_xyzvektor point)
{
	double	dist;

	dist = point.x * point.x + point.z * point.z;
	if (dist < 1 && point.y >= shape.maximum - EPSILON)
		return (set_vector(0, 1, 0, 0));
	else if (dist < 1 && point.y <= shape.minimum + EPSILON)
		return (set_vector(0, -1, 0, 0));
	else
		return (set_vector(point.x, 0, point.z, 0));
}

t_xyzvektor	calculate_normale(t_shape shape, t_xyzvektor point)
{
	t_xyzvektor	ret;
	double		**inverse_transform;
	double		**transpose_inverse_transform;
	t_xyzvektor	local_normal;

	if (shape.type == 0)
		return (sphere_normal(shape, point));
	else if (shape.type == 1)
	{
		inverse_transform = invert_matrix(shape.default_transformation, 4);
		local_normal = shape.normal;
		transpose_inverse_transform = transpose_matrix(inverse_transform, 4);
		free_double_ptr(inverse_transform, 4);
		ret = multiply_vector_and_matrix(local_normal,
				transpose_inverse_transform);
		free_double_ptr(transpose_inverse_transform, 4);
		return (ret);
	}
	else if (shape.type == 2)
		return (calc_cylinder_normal(shape, point));
	else if (shape.type == 3)
		return (calc_cone_normal(shape, point));
	return (set_vector(0, 0, 0, 0));
}

static double	get_shadow_factor(bool *in_shadow, t_c canvas)
{
	int		i;
	double	shadow_factor;

	shadow_factor = 0.1;
	i = -1;
	while (++i < canvas.num_lights)
	{
		if (in_shadow[i])
			shadow_factor += 0.5;
	}
	if (shadow_factor > 1)
		shadow_factor = 1;
	return (shadow_factor);
}

static t_xyzvektor	clamp_color(t_xyzvektor color)
{
	return ((t_xyzvektor){fmax(0, fmin(1, color.x)), fmax(0, fmin(1, color.y)),
		fmax(0, fmin(1, color.z))});
}

t_xyzvektor	lightning(t_shape shape, t_xyzvektor point, t_c canvas,
		bool *in_shadow)
{
	t_store		store;
	double		light_dot_normale;
	double		shadow_factor;
	int			i;
	t_xyzvektor	result;
	t_xyzvektor	scaled_spec;
	t_xyzvektor	final;

	i = -1;
	store.diffuse = set_black();
	store.specular = set_black();
	store.ambient = set_black();
	result = set_black();
	if (shape.material.checker_enable)
		store.materialcolor = pattern_at(shape, point);
	else
		store.materialcolor = get_color_from_uint(shape.material.color);
	shadow_factor = get_shadow_factor(in_shadow, canvas);
	store.ambient = scalar_multiplication(store.materialcolor,
			shape.material.ambient);
	while (++i < canvas.num_lights)
	{
		store.lightsourcecolor = canvas.lightsource[i].color;
		store.effective_color = hadamard_product(store.materialcolor,
				store.lightsourcecolor);
		store.light_vector = normalize(substraction(canvas.lightsource[i].position,
					point));
		light_dot_normale = dot_product(store.light_vector, canvas.normale);
		if (light_dot_normale >= 0)
		{
			store.diffuse = scalar_multiplication(store.materialcolor,
					shape.material.diffuse * light_dot_normale);
			result = addition(result, scalar_multiplication(store.diffuse, 1.0
						- shadow_factor));
			store.reflectv = calculate_reflection(store.light_vector,
					canvas.normale);
			store.reflect_dot_eye = dot_product(store.reflectv,
					negate_tuple(canvas.eyevector));
			if (store.reflect_dot_eye > 0)
			{
				store.factor = pow(store.reflect_dot_eye,
						shape.material.shininess);
				scaled_spec = scalar_multiplication(store.lightsourcecolor,
						shape.material.specular * store.factor * (1.0
							- shadow_factor));
				store.specular = addition(store.specular, scaled_spec);
			}
		}
	}
	final = addition(store.ambient, addition(result, store.specular));
	final.x = fmax(0.0, fmin(1.0, final.x));
	final.y = fmax(0.0, fmin(1.0, final.y));
	final.z = fmax(0.0, fmin(1.0, final.z));
	FREE(in_shadow);
	return (clamp_color(final));
}

// reflection_test
// int main(void)
// {
// 	t_sphere sphere = new_sphere();
// 	t_c canvas;
// 	t_xyzvektor point;
// 	t_xyzvektor eye;
// 	uint32_t colors;
// 	t_xyzvektor color;
// 	t_xyzvektor normale;

// 	init_canvas(&canvas);
// 	point.x = 0;
// 	point.y = 0;
// 	point.z = 0;
// 	point.w = 0;

// 	eye.x = 0;
// 	eye.y = 0;
// 	eye.z = - 1;

// 	normale.x = 0;
// 	normale.y = 0;
// 	normale.z = -1;

// 	canvas.eyevector = substraction(eye, point);
// 	canvas.normale = normale;

// 	printf("%f %f %f\n", canvas.eyevector.x, canvas.eyevector.y,
//		canvas.eyevector.z);
// 	color = lightning(sphere.material, point, canvas);

// 	printf("%f %f %f\n" , color.x, color.y, color.z);
// }
