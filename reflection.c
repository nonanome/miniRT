#include "miniRT.h"

t_xyzvektor calculate_reflection(t_xyzvektor in, t_xyzvektor normale)
{
	double dot;
	t_xyzvektor n;

	dot = 2 * dot_product(in, normale);
	n = scalar_multiplication(normale, dot);
	return normalize(substraction(in, n));
}

t_xyzvektor sphere_normal(t_shape shape, t_xyzvektor point)
{
	t_xyzvektor local_point;
	t_xyzvektor local_normal;
	t_xyzvektor world_normal;
	double **inverse_transform;
	double **transpose_inverse_transform;

	inverse_transform = invert_matrix(shape.default_transformation, 4);
	local_point = multiply_vector_and_matrix(point, inverse_transform);
	local_normal = substraction(local_point, shape.origin);
	transpose_inverse_transform = transpose_matrix(inverse_transform, 4);
	world_normal = multiply_vector_and_matrix(local_normal, transpose_inverse_transform);
	world_normal.w = 0;
	free_double_ptr(transpose_inverse_transform, 4);
	free_double_ptr(inverse_transform, 4);
	return normalize(world_normal);
}

t_xyzvektor calculate_normale(t_shape shape, t_xyzvektor point)
{
	t_xyzvektor ret;

	if (shape.type == 0)
		return sphere_normal(shape, point);
	else if (shape.type == 1)
		{
			double **inverse_transform;
			double **transpose_inverse_transform;
			t_xyzvektor local_normal;

			inverse_transform = invert_matrix
				(shape.default_transformation, 4);
			local_normal = shape.normal;
			transpose_inverse_transform = transpose_matrix
				(inverse_transform, 4);
			free_double_ptr(inverse_transform, 4);
			ret = multiply_vector_and_matrix(local_normal,
				transpose_inverse_transform);
			free_double_ptr(transpose_inverse_transform, 4);
			return (ret);
		}
	else if (shape.type == 2)
		return (set_vector(point.x, 0, point.z, 0));
}

t_xyzvektor lightning(t_shape shape, t_xyzvektor point, t_c canvas, bool in_shadow)
{
	t_store store;
    double light_dot_normale;

	if (shape.material.checker.enable)
		store.materialcolor = pattern_at(shape, point);
	else
		store.materialcolor = get_color_from_uint(shape.material.color);
	if (in_shadow)
	{
		return (scalar_multiplication(hadamard_product(store.materialcolor, canvas.lightsource.color), 0.1));
	}
	store.lightsourcecolor = canvas.lightsource.color;
    store.effective_color = hadamard_product(store.materialcolor, store.lightsourcecolor);
    store.light_vector = normalize(substraction(canvas.lightsource.position, point));
	store.ambient = scalar_multiplication(store.effective_color , shape.material.ambient);
    light_dot_normale = dot_product(store.light_vector, canvas.normale);
	store.diffuse = set_black();
    store.specular = set_black();
	if (light_dot_normale >= 0)
	{
        store.diffuse = scalar_multiplication(scalar_multiplication(store.effective_color, shape.material.diffuse) ,light_dot_normale);
        store.reflectv = calculate_reflection(store.light_vector, canvas.normale);
        store.reflect_dot_eye = dot_product(store.reflectv, negate_tuple(canvas.eyevector));
        if (store.reflect_dot_eye <= 0)
			store.specular = set_black();  
        else 
		{
            store.factor = pow(store.reflect_dot_eye, shape.material.shininess);
            store.specular = scalar_multiplication(scalar_multiplication(store.lightsourcecolor, shape.material.specular), store.factor);
        }
    }
    return addition(addition(store.ambient , store.diffuse)  ,store.specular);
}


//reflection_test
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

// 	printf("%f %f %f\n", canvas.eyevector.x, canvas.eyevector.y, canvas.eyevector.z);
// 	color = lightning(sphere.material, point, canvas);

// 	printf("%f %f %f\n" , color.x, color.y, color.z);
// }

