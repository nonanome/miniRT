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

void	*ft_calloc(int count, int size)
{
	char	*allocated_memory;

	allocated_memory = (void *) MALLOC (count * size);
	if (allocated_memory == 0)
		return (NULL);
	ft_memset(allocated_memory, 0, count * size);
	return (allocated_memory);
}

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

int main(void)
{
	t_c canvas;
	init_canvas(&canvas);
	mlx_loop_start(canvas);
}