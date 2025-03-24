/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:43:31 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/24 18:46:29 by qhahn            ###   ########.fr       */
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
	local_normal = substraction(local_point, shape.origin);
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
		return (set_vector(point.x, 0, point.z, 0));
	else if (shape.type == 3)
		return (calc_cone_normal(shape, point));
}

t_xyzvektor	lightning(t_shape shape, t_xyzvektor point, t_c canvas,
		bool *in_shadow)
{
	t_store		store;
	double		light_dot_normale;
	double		shadow_factor;
	int			i;
	t_xyzvektor	result;

	i = -1;
	store.diffuse = set_black();
	store.specular = set_black();
	store.ambient = set_black();
	if (shape.material.checker.enable)
		store.materialcolor = pattern_at(shape, point);
	else
		store.materialcolor = get_color_from_uint(shape.material.color);
	while (++i < canvas.num_lights)
	{
		shadow_factor = in_shadow[i] ? 0.1 : 1.0;
		store.lightsourcecolor = canvas.lightsource[i].color;
		store.effective_color = hadamard_product(store.materialcolor,
				store.lightsourcecolor);
		store.light_vector = normalize(substraction
				(canvas.lightsource[i].position, point));
		store.ambient = addition(store.ambient,
				scalar_multiplication(store.effective_color, shape.material.ambient));
		light_dot_normale = dot_product(store.light_vector, canvas.normale);
		if (light_dot_normale >= 0)
		{
			store.diffuse = addition(store.diffuse,
					scalar_multiplication(scalar_multiplication
						(store.effective_color,
							shape.material.diffuse), light_dot_normale
						* shadow_factor));
			store.reflectv = calculate_reflection(store.light_vector,
					canvas.normale);
			store.reflect_dot_eye = dot_product(store.reflectv,
					negate_tuple(canvas.eyevector));
			if (store.reflect_dot_eye > 0)
			{
				store.factor = pow(store.reflect_dot_eye, shape.material.shininess);
				store.specular = addition(store.specular,
						scalar_multiplication(scalar_multiplication
							(store.lightsourcecolor,
								shape.material.specular), store.factor
							* shadow_factor));
			}
		}
	}
	FREE(in_shadow);
	return (addition(addition(store.ambient, store.diffuse), store.specular));
	result.x = 1;
	result.y = 0;
	result.z = 0;
	return (result);
}

// t_xyzvektor lightning(t_material material, t_xyzvektor point, t_c canvas, bool *in_shadow)
// {
// 	t_store store;
//     double light_dot_normale;
// 	int i;

// 	i = -1;
// 	store.diffuse = set_black();
// 	store.specular = set_black();
// 	while(++ i < canvas.num_lights)
// 	{
// 		if (in_shadow[i])
// 		{
// 			return (scalarMultiplication(hadamard_product(get_color_from_uint(material.color), canvas.lightsource[i].color), 0.1));
// 		}
// 		store.materialcolor = get_color_from_uint(material.color);
// 		store.lightsourcecolor = canvas.lightsource[i].color;
// 		store.effective_color = hadamard_product(store.materialcolor, store.lightsourcecolor);
// 		store.light_vector = normalize(substraction(canvas.lightsource[i].position, point));
// 		store.ambient = scalarMultiplication(store.effective_color , material.ambient);
// 		light_dot_normale = dotProduct(store.light_vector, canvas.normale);
// 		if (light_dot_normale >= 0) 
// 		{
// 			store.diffuse = scalarMultiplication(scalarMultiplication(store.effective_color, material.diffuse) ,light_dot_normale);
// 			store.reflectv = calculate_reflection(store.light_vector, canvas.normale);
// 			store.reflect_dot_eye = dotProduct(store.reflectv, negateTuple(canvas.eyevector));
// 			if(store.reflect_dot_eye > 0) 
// 			{
// 				store.factor = pow(store.reflect_dot_eye, material.shininess);
// 				store.specular = scalarMultiplication(scalarMultiplication(store.lightsourcecolor, material.specular), store.factor);
// 			}
// 		}
// 	}


//     return addition(addition(store.ambient , store.diffuse)  ,store.specular);
// }

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
