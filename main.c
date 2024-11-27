#include "garbageCollector.h"
#include "miniRT.h"

static int globalID = 0;

typedef struct point_data
{
	xyzvektor position;
	xyzvektor velocity;
}	t_point;

typedef struct environment
{
	xyzvektor wind;
	xyzvektor gravity;
}	t_env;



typedef struct ray
{
	xyzvektor origin;
	xyzvektor direction;
}	t_ray;

typedef struct sphere
{
	int			id;
	xyzvektor	origin;
	double		radius;
}	t_sphere;

typedef struct intersect
{
	double *times;
	int		object_id;
}	t_intersec;

typedef struct intersections
{
	t_intersec *intersections;
	size_t		nr_intersections;
}	t_all_intersec;

typedef struct canvas
{
	int				width;
	int				height;
	mlx_image_t		*img;
	mlx_t			*mlx_ptr;
	t_all_intersec	all_intersections;
	
}	t_c;

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


int *calculate_pixel_coordinates(xyzvektor point)
{
	int *result;

	result = malloc(2 * sizeof(int));
	result[0] = point.x + 750;
	result[1] = point.y + 750;
	return result;
}



// void draw_on_img(t_c *canvas)
// {

// 		mlx_put_pixel(canvas->img, x, y, color);



// }




void	mlx_loop_start(t_c canvas)
{
	canvas.img = mlx_new_image(canvas.mlx_ptr, canvas.height, canvas.width);
	//draw_on_img(&canvas);
	mlx_image_to_window(canvas.mlx_ptr, canvas.img, 0, 0);
	mlx_loop(canvas.mlx_ptr);
}


void init_canvas(t_c *canvas)
{

	canvas->height = 1500;
	canvas->width = 1500;
	canvas->mlx_ptr = mlx_init(canvas->height, canvas->width, "miniRT", false);
	canvas->all_intersections.nr_intersections = 0;
}

xyzvektor ray_position(t_ray ray, double time)
{
	xyzvektor result;

	result = addition(ray.origin, scalarMultiplication(ray.direction, time));
	return result; 
}

bool intersection_comparison_1_bigger_2(t_intersec *intersection1, t_intersec *intersection2)
{
	double smallest_time_of_1;
	double smallest_time_of_2;

	if(intersection1->times == NULL && intersection2->times == NULL)
		return false;
	if(intersection1->times == NULL)
		return true;
	if(intersection2->times == NULL)
		return false;
	if(intersection1->times[0] < intersection1->times[1])
		smallest_time_of_1 = intersection1->times[0];
	else
		smallest_time_of_1 = intersection1->times[1];
	if(intersection2->times[0] < intersection2->times[1])
		smallest_time_of_2 = intersection2->times[0];
	else
		smallest_time_of_2 = intersection2->times[1];
	if(smallest_time_of_1 < smallest_time_of_2)
		return false;
	else
		return true;
}

void	save_intersection(t_c *canvas, t_intersec *new_intersection, int i , int j)
{
	int nr_intersec;
	t_intersec *old_intersections;
	t_intersec *intersections_sorted;
	bool new_intersection_sorted;

	new_intersection_sorted = false;
	old_intersections = canvas->all_intersections.intersections;
	nr_intersec = canvas->all_intersections.nr_intersections + 1;
	canvas->all_intersections.nr_intersections = nr_intersec;
	intersections_sorted = malloc (nr_intersec * sizeof(t_intersec));
	if(nr_intersec == 1)
	{
		canvas->all_intersections.intersections = new_intersection;	
		return;
	}
	while(++ i < nr_intersec)
	{
		if((intersection_comparison_1_bigger_2(&old_intersections[j],new_intersection) && !new_intersection_sorted) || (i == nr_intersec - 1 && !new_intersection_sorted))
		{
			intersections_sorted[i] = *new_intersection;
			new_intersection_sorted = true;
		}
		else
		{
			intersections_sorted[i] = old_intersections[j];
			j ++;
		}
	}
	canvas->all_intersections.intersections = intersections_sorted;
}

t_sphere new_sphere()
{
	t_sphere result;

	result.origin.x = 0;
	result.origin.y = 0;
	result.origin.z = 0;
	result.origin.w = 1;
	result.radius = 1;
	result.id = globalID;
	globalID ++;
	return result;
}

double get_discriminant(double *discriminant_values)
{
	return (discriminant_values[1] * discriminant_values[1] - 4
		* discriminant_values[0] * discriminant_values[2]);
}

t_intersec *intersect(t_sphere sphere, t_ray ray)
{
	xyzvektor sphere_to_ray;
	t_intersec	*result;
	double *discriminant_values;
	double discriminant;

	result = malloc(sizeof(t_intersec));
	discriminant_values = malloc(3 * sizeof(double));
	sphere_to_ray = substraction(ray.origin, sphere.origin);
	discriminant_values[0] = dotProduct(ray.direction, ray.direction);
	discriminant_values[1] = 2 * dotProduct(ray.direction, sphere_to_ray);
	discriminant_values[2] = dotProduct(sphere_to_ray, sphere_to_ray) - 1;
	discriminant = get_discriminant(discriminant_values);
	if(discriminant < 0)
		return NULL;
	result->times = malloc (2 * sizeof(double));
	result->times[0] = (-discriminant_values[1] - sqrt(discriminant)) / (2 * discriminant_values[0]);
	result->times[1] = (-discriminant_values[1] + sqrt(discriminant)) / (2 * discriminant_values[0]);
	result->object_id = sphere.id;
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

int main(void)
{
	t_ray ray;
	t_ray ray2;
	xyzvektor origin;
	xyzvektor origin2;
	xyzvektor direction;
	t_c canvas;

	init_canvas(&canvas);
	origin.x = 0;
	origin.y = 0;
	origin.z = -5;
	origin.w = 1;

	direction.x = 0;
	direction.y = 0;
	direction.z = 1;
	direction.w = 0;

	origin2.x = 0;
	origin2.y = 1;
	origin2.z = -5;
	origin2.w = 1;

	ray.origin = origin;
	ray.direction = direction;

	ray2.origin = origin2;
	ray2.direction = direction;

	t_sphere sphere1 = new_sphere();
	t_sphere sphere2 = new_sphere();

	t_intersec *sphere_intersections2 = intersect(sphere1, ray2);
	t_intersec *sphere_intersections = intersect(sphere2, ray);

	save_intersection(&canvas, sphere_intersections2, -1, 0);
	save_intersection(&canvas, sphere_intersections, -1, 0);

	printf("%f %f", sphere_intersections2->times[0], sphere_intersections2->times[1]);
	printf("\n");
	printf("%d  ",  canvas.all_intersections.nr_intersections);
	printf("%f %f %f %f", canvas.all_intersections.intersections[0].times[0], canvas.all_intersections.intersections[0].times[1], canvas.all_intersections.intersections[1].times[0], canvas.all_intersections.intersections[1].times[1]);
}

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