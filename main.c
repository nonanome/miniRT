#include "garbageCollector.h"
#include "miniRT.h"

// int	point_out_bounds(double x, double y, t_c canvas)
// {
// 	if (x > canvas.height | y > canvas.width)
// 		return (1);
// 	return (0);
// }



xyzvektor ray_position(t_ray ray, double time)
{
	xyzvektor result;

	result = addition(ray.origin, scalarMultiplication(ray.direction, time));
	return result; 
}








//int main(void)
//{


//	t_c canvas;


//	init_canvas(&canvas);
//	mlx_loop_hook(canvas.mlx_ptr, &visualize, (void *) &canvas);
//	mlx_loop(canvas.mlx_ptr);

//}

// int main(void)
// {
// 	t_c canvas;
// 	init_canvas(&canvas);
// //	mlx_loop_start(canvas);
// }

// t_intersec hit(t_c canvas)
// {
// 	t_intersec smallest;
// 	int i;

// 	i = 1;
// 	smallest = canvas.all_intersections.intersections[0];
// 	while(i < canvas.all_intersections.nr_intersections)
// 	{
// 		if()
// 	}
// }

xyzvektor calculate_wall_coordinate(int x, int y, double pixel_size, double half)
{
	xyzvektor result;
	result.x = x / pixel_size - half;
	result.y = half - y / pixel_size;
	result.z = 10;
	result.w = 1;
	return result;
}







// int main(void)
// {
// 	t_sphere sphere = new_sphere();
// 	xyzvektor point;

// 	point.x = 0;
// 	point.y = 1;
// 	point.z = 0;

// 	xyzvektor normal = calculate_normale_of_sphere(sphere, point);

// 	printf("%f %f %f", normal.x, normal.y, normal.z);
// }







// void visualize(void *input)
// {
// 	t_c *canvas = input;
// 	t_ray ray = init_ray();
// 	v_s vs_data;
// 	t_shape sphere1 = new_shape(0);

// 	int x = 0;
// 	int y = 0;
// 	mlx_delete_image(canvas->mlx_ptr, canvas->img);
// 	canvas->img = mlx_new_image(canvas->mlx_ptr, canvas->height, canvas->width);
// 	while(y < canvas->height)
// 	{
// 		while(x < canvas->width)
// 		{
// 			vs_data.world_coordinates = calculate_wall_coordinate(x, y, canvas->pixel_size, canvas->half_size);
// 			ray.direction = normalize(substraction(vs_data.world_coordinates, ray.origin));
// 			canvas->eyevector = negateTuple(ray.direction);
// 			vs_data.intersec = intersect(sphere1, ray);
// 			if(vs_data.intersec != NULL)
// 			{
// 				vs_data.intersectionpoint = point_of_intersection(vs_data.intersec, ray);
// 				canvas->normale = calculate_normale(sphere1, vs_data.intersectionpoint);
// 				vs_data.color = lightning(sphere1.material, vs_data.intersectionpoint, *canvas, 0);
// 				mlx_put_pixel(canvas->img, x, y, get_color_from_tuple(vs_data.color));
// 			}
// 			else
// 				mlx_put_pixel(canvas->img, x, y, 0x000000);
// 			free(vs_data.intersec);
// 			x ++;
// 		}
// 		x = 0;
// 		y ++;
// 	}
// 	mlx_image_to_window(canvas->mlx_ptr, canvas->img, 0, 0);
// }





// int main(void)
// {
// 	xyzvektor a;
// 	xyzvektor normale;
// 	xyzvektor reflect;

// 	a.x = 0;
// 	a.y = -1;
// 	a.z = 0;

// 	normale.x = sqrt(2) / 2;
// 	normale.y = sqrt(2) / 2;
// 	normale.z = 0;

// 	reflect = calculate_reflection(a, normale);

// 	printf("%f %f %f", reflect.x, reflect.y, reflect.z);
// }

// int main(void)
// {
// 	t_ray ray;
// 	t_ray ray2;
// 	xyzvektor origin;
// 	xyzvektor origin2;
// 	xyzvektor direction;
// 	t_c canvas;

// 	init_canvas(&canvas);
// 	origin.x = 0;
// 	origin.y = 0;
// 	origin.z = -5;
// 	origin.w = 1;

// 	direction.x = 0;
// 	direction.y = 0;
// 	direction.z = 1;
// 	direction.w = 0;

// 	origin2.x = 0;
// 	origin2.y = 1;
// 	origin2.z = 5;
// 	origin2.w = 1;

// 	ray.origin = origin;
// 	ray.direction = direction;

// 	ray2.origin = origin2;
// 	ray2.direction = direction;

// 	t_sphere sphere1 = new_sphere();
// 	t_sphere sphere2 = new_sphere();

// 	t_intersec *sphere_intersections2 = intersect(sphere1, ray2);
// 	t_intersec *sphere_intersections = intersect(sphere2, ray);

// 	save_intersection(&canvas, sphere_intersections2, -1, 0);
// 	save_intersection(&canvas, sphere_intersections, -1, 0);

// 	printf("%f %f", sphere_intersections2->times[0], sphere_intersections2->times[1]);
// 	printf("\n");
// 	printf("%f %f    ", canvas.all_intersections.intersections[0].times[0], canvas.all_intersections.intersections[0].times[1]);
// 	printf("%f %f %f", identify_hit(canvas.all_intersections).x, identify_hit(canvas.all_intersections).y, identify_hit(canvas.all_intersections).z);

// }

// int main(void)
// {
// 	double **a;
// 		double **b;
// 	int i;
// 	int j;

// 	a = malloc(4 * (sizeof(double *)));
// 	i = -1;
// 	while(++ i < 4)
// 		a[i] = malloc(4 * (sizeof(double)));

// 	b = malloc(4 * (sizeof(double *)));
// 	i = -1;
// 	while(++ i < 4)
// 		b[i] = malloc(4 * (sizeof(double)));
	
// 	// a[0][0] = 1;
// 	// a[0][1] = 2;
// 	// a[0][2] = 6;
// 	// a[1][0] = -5;
// 	// a[1][1] = 8;
// 	// a[1][2] = -4;
// 	// a[2][0] = 2;
// 	// a[2][1] = 6;
// 	// a[2][2] = 4;

// 	a[0][0] = 3;
// 	a[0][1] = -9;
// 	a[0][2] = 7;
// 	a[0][3] = 3;
// 	a[1][0] = 3;
// 	a[1][1] = -8;
// 	a[1][2] = 2;
// 	a[1][3] = -9;
// 	a[2][0] = -4;
// 	a[2][1] = 4;
// 	a[2][2] = 4;
// 	a[2][3] = 1;
// 	a[3][0] = -6;
// 	a[3][1] = 5;
// 	a[3][2] = -1;
// 	a[3][3] = 1;

// 	b[0][0] = 8;
// 	b[0][1] = 2;
// 	b[0][2] = 2;
// 	b[0][3] = 2;
// 	b[1][0] = 3;
// 	b[1][1] = -1;
// 	b[1][2] = 7;
// 	b[1][3] = 0;
// 	b[2][0] = 7;
// 	b[2][1] = 0;
// 	b[2][2] = 5;
// 	b[2][3] = 4;
// 	b[3][0] = 6;
// 	b[3][1] = -2;
// 	b[3][2] = 0;
// 	b[3][3] = 5;

// 	double **c = invert_matrix(b, 4);
// 	double **d = multiply_matrix(a, b);
// 	double **e = multiply_matrix(d, c);
// 	show_matrix(e, 4);
// 	// get_cofactor(a, 0, 1, 3);
// 	// get_cofactor(a, 0, 2, 3);
// 	// printf("%f",get_minor(a, 0, 0, 3));
// 	//show_matrix(get_submatrix(a, 0, 0, 3), 2);
// }

// int main(void)
// {
// 	xyzvektor a;

// 	a.x = 2;
// 	a.y = 3;
// 	a.z = 4;
// 	a.w = 1;

// 	double *proportion = malloc(6 * sizeof(double));
// 	proportion[0] = 0;
// 	proportion[1] = 0;
// 	proportion[2] = 0;
// 	proportion[3] = 0; 
// 	proportion[4] = 0;
// 	proportion[5] = 1;
// 	double **scale = shearing(proportion);
// 	xyzvektor result = multiply_vector_and_matrix(a, scale);

// 	printf("%f\n", result.x);
// 	printf("%f\n", result.y);
// 	printf("%f\n", result.z);
// }