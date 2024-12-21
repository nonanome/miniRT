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

int min_of(int a, int b)
{
	if(a < b)
		return a;
	return b;
}

uint32_t	get_color_from_tuple(xyzvektor color)
{
    // uint8_t r = min_of(color.x * 255, 255);
    // uint8_t g = min_of(color.y * 255, 255);
    // uint8_t b = min_of(color.z * 255, 255);

    uint8_t r = color.x;
    uint8_t g = color.y;
    uint8_t b = color.z;

    // Combine into a single 32-bit integer (0xRRGGBB)
    return (r << 16) | (g << 8) | b;
}

xyzvektor	get_color_from_uint(uint32_t color)
{
	xyzvektor result;
	uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    result.x = (double)r;
    result.y = (double)g;
    result.z = (double)b;
	result.w = 1;

    return result;
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
	canvas->lightsource = default_light();
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

xyzvektor set_black(void)
{
	xyzvektor result;

	result.x = 0;
	result.y = 0;
	result.z = 0;
	result.w = 1;
	return result;
}



xyzvektor lightning(t_material material, xyzvektor point, t_c canvas) {
	t_store store;
    double light_dot_normale;

	// printf("%f %f %f\n", point.x, point.y, point.z);
	store.materialcolor = get_color_from_uint(material.color);
	store.lightsourcecolor = canvas.lightsource.color;
    store.effective_color = hadamard_product(store.materialcolor, store.lightsourcecolor);
    store.light_vector = normalize(substraction(canvas.lightsource.position, point));
	store.ambient = scalarMultiplication(store.effective_color , material.ambient);
	// printf("%f %f %f\n", canvas.normale.x, canvas.normale.y, canvas.normale.z);
	// printf("%f %f %f\n",store.light_vector.x,store.light_vector.y,store.light_vector.z);
    light_dot_normale = dotProduct(store.light_vector, canvas.normale);
	// printf("     %f\n", light_dot_normale);
	// printf("%d\n", get_color_from_tuple(store.ambient));
		store.diffuse = set_black();
        store.specular = set_black();
	if (light_dot_normale >= 0) 
	{
        store.diffuse = scalarMultiplication(scalarMultiplication(store.effective_color, material.diffuse) ,light_dot_normale);
		// printf("%d\n", get_color_from_tuple(store.diffuse));
        store.reflectv = calculate_reflection(store.light_vector, canvas.normale);

		// printf("%f\n", store.light_vector.z);
		// 		printf("%f\n", canvas.normale.z);
		// 				printf("%f\n\n", canvas.eyevector.z);
        store.reflect_dot_eye = dotProduct(store.reflectv, negateTuple(canvas.eyevector));
        if (store.reflect_dot_eye <= 0)
		{
		//	printf("test\n\n");
			store.specular = set_black();
		}
            
        else 
		{
				//printf("test\n");
            store.factor = pow(store.reflect_dot_eye, material.shininess);
            store.specular = scalarMultiplication(scalarMultiplication(store.lightsourcecolor , material.specular), store.factor);
			//printf("  %d\n", get_color_from_tuple(store.specular));
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

int main(void)
{


	t_c canvas;


	init_canvas(&canvas);
	mlx_loop_hook(canvas.mlx_ptr, &visualize, (void *) &canvas);
	mlx_loop(canvas.mlx_ptr);

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

xyzvektor calculate_normale_of_sphere(t_sphere sphere, xyzvektor point)
{
	xyzvektor object_normale;
	xyzvektor world_normale;
	xyzvektor point_object_space;
	double **transpose;


	// transpose = transpose_matrix(invert_matrix(sphere.default_transformation, 4), 4);
	// point_object_space = multiply_vector_and_matrix(point, invert_matrix(sphere.default_transformation, 4));
	// object_normale = substraction(point_object_space,  sphere.origin);
	// world_normale = multiply_vector_and_matrix(object_normale, transpose);
	// world_normale.w = 0;
		transpose = transpose_matrix(invert_matrix(sphere.default_transformation, 4), 4);
	point_object_space = multiply_vector_and_matrix(point, invert_matrix(sphere.default_transformation, 4));
	object_normale = substraction(point_object_space,  sphere.origin);
	world_normale = multiply_vector_and_matrix(object_normale, transpose);
	world_normale.w = 0;
	return normalize(world_normale);
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



xyzvektor calculate_reflection(xyzvektor in, xyzvektor normale)
{
	double dot;
	xyzvektor n;

	dot = 2 * dotProduct(in, normale);
	n = scalarMultiplication(normale, dot);
	return normalize(substraction(in, n));
}



t_material default_material(void)
{
	t_material dm;
	xyzvektor color;

	color.x = 255;
	color.y = 255;
	color.z = 255;
	color.w = 1;

	dm.color = get_color_from_tuple(color);
	dm.ambient = 0.1;
	dm.diffuse = 0.9;
	dm.specular = 0.9;
	dm.shininess = 500.0;
	return dm;
}

t_light default_light(void)
{
	t_light source;
	xyzvektor color;
	xyzvektor position;

	color.x = 1;
	color.y = 1;
	color.z = 1;
	color.w = 1;

	position.x = -10;
	position.y = 10;
	position.z = -10;
	position.w = 0;

	source.color = color;
	source.position = position;
	return source;
}


void visualize(void *input)
{
	t_c *canvas = input;
	t_ray ray = init_ray();
	t_intersec *intersec;
	t_sphere sphere1 = new_sphere();
	xyzvektor world_coordinates;
	xyzvektor light_vector;
	xyzvektor eyevector;
	xyzvektor surfacenormale;
	xyzvektor reflectionvector;
	xyzvektor intersectionpoint;


	int x = 0;
	int y = 0;

	mlx_delete_image(canvas->mlx_ptr, canvas->img);
	canvas->img = mlx_new_image(canvas->mlx_ptr, canvas->height, canvas->width);
	while(y < canvas->height)
	{
		while(x < canvas->width)
		{
			world_coordinates = calculate_wall_coordinate(x, y, canvas->pixel_size, canvas->half_size);
			ray.direction = normalize(substraction(world_coordinates, ray.origin));
			canvas->eyevector = negateTuple(ray.direction);
			intersec = intersect(sphere1, ray);
			if(intersec != NULL)
			{
				intersectionpoint = point_of_intersection(intersec, ray);
				// printf("%f %f %f\n",intersectionpoint.x,intersectionpoint.y,intersectionpoint.z);
				canvas->normale = calculate_normale_of_sphere(sphere1, intersectionpoint);
				xyzvektor color = lightning(sphere1.material, intersectionpoint, *canvas);
				// printf("%f %f %f\n", color.x, color.y, color.z);
				//printf("%d\n", get_color_from_tuple(color));
				mlx_put_pixel(canvas->img, x, y, get_color_from_tuple(color));
			}
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