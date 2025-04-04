/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkuhn <kkuhn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:43:31 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/04 18:41:44 by kkuhn            ###   ########.fr       */
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

t_xyzvektor	add_light(t_store *store, t_shape shape, t_xyzvektor *result,
		t_c canvas)
{
	t_xyzvektor	scaled_spec;

	store->diffuse = scalar_multiplication(store->materialcolor,
			shape.material.diffuse * store->light_dot_normale);
	*result = addition(*result, scalar_multiplication(store->diffuse, 1.0
				- store->shadow_factor));
	store->reflectv = calculate_reflection(store->light_vector, canvas.normale);
	store->reflect_dot_eye = dot_product(store->reflectv,
			negate_tuple(canvas.eyevector));
	if (store->reflect_dot_eye > 0)
	{
		store->factor = pow(store->reflect_dot_eye, shape.material.shininess);
		scaled_spec = scalar_multiplication(store->lightsourcecolor,
				shape.material.specular * store->factor * (1.0
					- store->shadow_factor));
		store->specular = addition(store->specular, scaled_spec);
	}
	return (*result);
}

t_xyzvektor	each_light(t_store *store, t_shape shape, t_c canvas,
		t_xyzvektor point)
{
	t_xyzvektor	result;
	t_xyzvektor	scaled_spec;
	int			i;

	i = -1;
	result = set_black();
	while (++i < canvas.num_lights)
	{
		store->lightsourcecolor = canvas.lightsource[i].color;
		store->effective_color = hadamard_product(store->materialcolor,
				store->lightsourcecolor);
		store->light_vector = normalize(substraction
				(canvas.lightsource[i].position, point));
		store->light_dot_normale = dot_product(store->light_vector,
				canvas.normale);
		if (store->light_dot_normale >= 0)
		{
			result = add_light(store, shape, &result, canvas);
		}
	}
	return (result);
}

t_xyzvektor get_color(t_c canvas, t_shape shape, int x, int y)
{
	t_xyzvektor color;
	
	// if(canvas.bumpmapcolor)
	// {
	// 	x = (x < 0) ? 0 : ((x >= canvas.bumpmapcolor->width) ? canvas.bumpmapcolor->width - 1 : x);
	// 	y = (y < 0) ? 0 : ((y >= canvas.bumpmapcolor->height) ? canvas.bumpmapcolor->height - 1 : y);
		
	// 	// Pixeladresse berechnen (MLX speichert Pixel als uint32_t)
	// 	int pixel_offset = y * (canvas.bumpmapcolor->width * 4 / sizeof(uint32_t)) + x;
	// 	uint32_t pixel = ((uint32_t *)canvas.bumpmapcolor->pixels)[pixel_offset];
		
	// 	// Farbe aus ARGB extrahieren (MLX-Format ist normalerweise 0xAARRGGBB)
	// 	color.x = (pixel >> 24) & 0xFF;  // Alpha (falls benötigt)
	// 	color.y = (pixel >> 16) & 0xFF;  // Rot
	// 	color.z = (pixel >> 8)  & 0xFF;  // Grün
	// 	color.w = pixel         & 0xFF;  // Blau
		
	// 	return color;
	// }
	// else
		return get_color_from_uint(shape.material.color);
}

t_xyzvektor	lightning(t_comp comp, t_c canvas,
		bool *in_shadow)
{
	t_store		store;
	t_xyzvektor	result;
	t_xyzvektor	final;
	t_shape shape;
	t_xyzvektor point;

	shape = *(comp.object);
	point = comp.over_point;
	store.diffuse = set_black();
	store.specular = set_black();
	store.ambient = set_black();
	if (shape.material.checker_enable)
		store.materialcolor = pattern_at(shape, point);
	else
		store.materialcolor = get_color(canvas, shape, (int)comp.u, (int)comp.v);
	store.shadow_factor = get_shadow_factor(in_shadow, canvas);
	store.ambient = scalar_multiplication(store.materialcolor,
			shape.material.ambient);
	result = each_light(&store, shape, canvas, point);
	final = addition(store.ambient, addition(result, store.specular));
	FREE(in_shadow);
	final.x = fmax(0.0, fmin(1.0, final.x));
	final.y = fmax(0.0, fmin(1.0, final.y));
	final.z = fmax(0.0, fmin(1.0, final.z));
	return (final);
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
