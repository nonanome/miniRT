#include "miniRT.h"

void init_canvas(t_c *canvas)
{
	canvas->worldheight = 8;
	canvas->height = 800;
	canvas->width = 400;
	canvas->mlx_ptr = mlx_init(canvas->height, canvas->width, "miniRT", false);
	canvas->pixel_size = canvas->height / canvas->worldheight;
	canvas->half_size = canvas->worldheight / 2;
	canvas->all_intersections.nr_intersections = 0;
	canvas->all_intersections.intersections = NULL;
	canvas->lightsource = default_light();
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
