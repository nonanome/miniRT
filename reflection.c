#include "miniRT.h"

xyzvektor calculate_reflection(xyzvektor in, xyzvektor normale)
{
	double dot;
	xyzvektor n;

	dot = 2 * dotProduct(in, normale);
	n = scalarMultiplication(normale, dot);
	return normalize(substraction(in, n));
}

xyzvektor calculate_normale_of_sphere(t_sphere sphere, xyzvektor point)
{
	xyzvektor object_normale;
	xyzvektor world_normale;
	xyzvektor point_object_space;
	double **transpose;

	transpose = transpose_matrix(invert_matrix(sphere.default_transformation, 4), 4);
	point_object_space = multiply_vector_and_matrix(point, invert_matrix(sphere.default_transformation, 4));
	object_normale = substraction(point_object_space,  sphere.origin);
	world_normale = multiply_vector_and_matrix(object_normale, transpose);
	world_normale.w = 0;
	return normalize(world_normale);
}

xyzvektor lightning(t_material material, xyzvektor point, t_c canvas)
{
	t_store store;
    double light_dot_normale;

	store.materialcolor = get_color_from_uint(material.color);
	store.lightsourcecolor = canvas.lightsource.color;
    store.effective_color = hadamard_product(store.materialcolor, store.lightsourcecolor);
    store.light_vector = normalize(substraction(canvas.lightsource.position, point));
	store.ambient = scalarMultiplication(store.effective_color , material.ambient);
    light_dot_normale = dotProduct(store.light_vector, canvas.normale);
	store.diffuse = set_black();
    store.specular = set_black();
	if (light_dot_normale >= 0) 
	{
        store.diffuse = scalarMultiplication(scalarMultiplication(store.effective_color, material.diffuse) ,light_dot_normale);
        store.reflectv = calculate_reflection(store.light_vector, canvas.normale);
        store.reflect_dot_eye = dotProduct(store.reflectv, negateTuple(canvas.eyevector));
        if (store.reflect_dot_eye <= 0)
			store.specular = set_black();  
        else 
		{
            store.factor = pow(store.reflect_dot_eye, material.shininess);
            store.specular = scalarMultiplication(scalarMultiplication(store.lightsourcecolor, material.specular), store.factor);
        }
    }
    return addition(addition(store.ambient , store.diffuse)  ,store.specular);
}


//reflection_test
// int main(void)
// {
// 	t_sphere sphere = new_sphere();
// 	t_c canvas;
// 	xyzvektor point;
// 	xyzvektor eye;
// 	uint32_t colors;
// 	xyzvektor color;
// 	xyzvektor normale;

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

