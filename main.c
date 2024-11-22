#include "garbageCollector.h"
#include "miniRT.h"

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

typedef struct canvas
{
	int width;
	int height;
	mlx_image_t *img;
	mlx_t		*mlx_ptr;
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

void draw_on_img(t_c *canvas)
{
	xyzvektor colorTuple;
	int32_t color;
	int x;
	int y;

	x = -1;
	y = -1;
	colorTuple = set_vector(255, 255, 255, 0);
	while (++x < canvas->width)
	{
		while (++y < canvas->height)
		{
			color = get_color_from_tuple(colorTuple);
			if (point_out_bounds(x, y, *canvas) != 1)
				mlx_put_pixel(canvas->img, x, y, color);
		}
		y = -1;
	}
}

void	mlx_loop_start(t_c canvas)
{
	canvas.img = mlx_new_image(canvas.mlx_ptr, canvas.height, canvas.width);
	draw_on_img(&canvas);
	mlx_image_to_window(canvas.mlx_ptr, canvas.img, 0, 0);
	mlx_loop(canvas.mlx_ptr);
}


void init_canvas(t_c *canvas)
{

	canvas->height = 1500;
	canvas->width = 1500;
	canvas->mlx_ptr = mlx_init(canvas->height, canvas->width, "miniRT", false);
}

// int main(void)
// {
// 	t_c canvas;
// 	init_canvas(&canvas);
// 	mlx_loop_start(canvas);
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

int main(void)
{
	xyzvektor a;

	a.x = 2;
	a.y = 3;
	a.z = 4;
	a.w = 1;

	double *proportion = malloc(6 * sizeof(double));
	proportion[0] = 0;
	proportion[1] = 0;
	proportion[2] = 0;
	proportion[3] = 0; 
	proportion[4] = 0;
	proportion[5] = 1;
	double **scale = shearing(proportion);
	xyzvektor result = multiply_vector_and_matrix(a, scale);

	printf("%f\n", result.x);
	printf("%f\n", result.y);
	printf("%f\n", result.z);
}