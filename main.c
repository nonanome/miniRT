#include "garbageCollector.h"
#include "miniRT.h"

static int globalID = 0;



// void	*ft_calloc(int count, int size)
// {
// 	char	*allocated_memory;

// 	allocated_memory = (void *) MALLOC (count * size);
// 	if (allocated_memory == 0)
// 		return (NULL);
// 	ft_memset(allocated_memory, 0, count * size);
// 	return (allocated_memory);
// }

int	point_out_bounds(double x, double y, t_c canvas)
{
	if (x > canvas.height | y > canvas.width)
		return (1);
	return (0);
}

uint32_t	get_color_from_tuple(xyzvektor color)
{
	int	r;
	int	g;
	int	b;

	r = (int)color.x;
	g = (int)color.y;
	b = (int)color.z;
	return (r << 16 | g << 8 | b);
}



// void draw_on_img(t_c *canvas)
// {

// 		mlx_put_pixel(canvas->img, x, y, color);



// }





void init_canvas(t_c *canvas)
{
	canvas->worldheight = 8;
	canvas->height = 320;
	canvas->width = 320;
	canvas->mlx_ptr = mlx_init(canvas->height, canvas->width, "miniRT", false);
	canvas->pixel_size = canvas->height / canvas->worldheight;
	canvas->half_size = canvas->worldheight / 2;
	canvas->all_intersections.nr_intersections = 0;
}

xyzvektor ray_position(t_ray ray, double time)
{
	xyzvektor result;

	result = addition(ray.origin, scalarMultiplication(ray.direction, time));
	return result; 
}

t_sphere new_sphere()
{
	t_sphere result;

	result.origin.x = 0;
	result.origin.y = 0;
	result.origin.z = 0;
	result.origin.w = 1;
	result.radius = 1;
	result.default_transformation = get_identity_matrix();
	result.id = globalID;
	result.material = default_material();
	globalID ++;
	return result;
}



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

t_ray init_ray(void)
{
	t_ray ray;
	xyzvektor origin;
	xyzvektor direction;

	origin.x = 0;
	origin.y = 0;
	origin.z = -5;
	origin.w = 1;

	direction.x = 0;
	direction.y = 0;
	direction.z = 1;
	direction.w = 0;

	ray.origin = origin;
	ray.direction = direction;
	return ray;
}

xyzvektor point_of_intersection(t_intersec *intersec, t_ray ray)
{
	double time_of_intersection;
	xyzvektor way;
	xyzvektor point_of_intersection;

	time_of_intersection = get_smallest_positive_value(intersec);
	way = scalarMultiplication(ray.direction , time_of_intersection);
	point_of_intersection = addition(ray.origin, way);
	return point_of_intersection;
}

//maybe normalize
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
	return (normalize(world_normale));
}

xyzvektor calculate_reflection(xyzvektor in, xyzvektor normale)
{
	double dot;
	xyzvektor n;

	dot = 2 * dotProduct(in, normale);
	n = scalarMultiplication(normale, dot);
	return substraction(in, n);
}



t_material default_material(void)
{
	t_material dm;
	xyzvektor color;

	color.x = 1;
	color.y = 1;
	color.z = 1;

	dm.color = get_color_from_tuple(color);
	dm.ambient = 0.1;
	dm.diffuse = 0.9;
	dm.specular = 0.9;
	dm.shininess = 200.0;
}

t_light default_light(void)
{
	t_light source;
	xyzvektor color;
	xyzvektor position;

	color.x = 1;
	color.y = 1;
	color.z = 1;

	position.x = 0;
	position.y = 0;
	position.w = 0;

	source.color = get_color_from_tuple(color);
	source.position = position;
}


void visualize(void *input)
{
	t_c *canvas = input;
	t_ray ray = init_ray();
	t_intersec *intersec;
	t_sphere sphere1 = new_sphere();
	xyzvektor world_coordinates;


	int x = 0;
	int y = 0;

	mlx_delete_image(canvas->mlx_ptr, canvas->img);
	canvas->img = mlx_new_image(canvas->mlx_ptr, canvas->height, canvas->width);
	while(y < canvas->height)
	{
		while(x < canvas->width)
		{
			world_coordinates = calculate_wall_coordinate(x, y, canvas->pixel_size, canvas->half_size);
			ray.direction = normalize(substraction(ray.origin, world_coordinates));
			intersec = intersect(sphere1, ray);
			if(intersec != NULL)
				mlx_put_pixel(canvas->img, x, y, 0xFFFFFF);
			else
				mlx_put_pixel(canvas->img, x, y, 0x000000);
			free(intersec);
			x ++;
		}
		x = 0;
		y ++;
	}
	mlx_image_to_window(canvas->mlx_ptr, canvas->img, 0, 0);
}



int main(void)
{


	t_c canvas;


	init_canvas(&canvas);
	mlx_loop_hook(canvas.mlx_ptr, &visualize, (void *) &canvas);
	mlx_loop(canvas.mlx_ptr);

}

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