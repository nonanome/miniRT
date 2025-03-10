#include "world.h"
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

t_material	set_material(xyzvektor color, double ambient, double diffuse,
		double specular, double shininess)
{
	t_material	material;

	material.color = get_color_from_tuple(color);
	material.ambient = ambient;
	material.diffuse = diffuse;
	material.specular = specular;
	material.shininess = shininess;
	return (material);
}

int	make_test_world(t_world *world)
{
	xyzvektor	color;
	xyzvektor	color2;
	xyzvektor	color3;
	xyzvektor	color4;

	color = set_vector(0.5, 1.0, 0.1, 1.0);
	color2 = set_vector(0.1, 1.0, 0.5, 1.0);
	color3 = set_vector(1.0, 0.8, 0.1, 1.0);
	color4 = set_vector(1.0, 0.9, 0.9, 1.0);
	if (!world)
		return (1);
	init_canvas(world->canvas);
	// //floor
	world->shapes[0] = new_shape(1);
	world->shapes[0]->material = set_material(color4, 0.1, 0.9, 0.9, 200.0);
	// ceiling
	world->shapes[1] = new_shape(1);
	world->shapes[1]->default_transformation = translation(0, 5, 0);
	world->shapes[1]->material = set_material(color4, 0.1, 0.9, 0.9, 200.0);
	// sphere	
	world->shapes[2] = new_shape(0);
	world->shapes[2]->default_transformation = translation(0, 5, 0);
	world->shapes[2]->material = set_material(color, 0.1, 0.7, 0.3, 200.0);
	world->nr_shapes = 3;
	world->all_sorted = calloc(sizeof(double *) ,100);
	if (!world->all_sorted)
		return (1);
	world->all_sorted[0] = 0;
	return (0);
}

int	main(void)
{
	t_world			*world;
	t_ray			ray;
	t_comp			comp;
	xyzvektor		shadestuff;
	double			**retmatrix;
	double			**rot;
	double			**trans;
	mlx_image_t		*image;
	unsigned int	color;
	xyzvektor		color2;

	world = get_world(6);
	if (!world)
		return (1);
	if (make_test_world(world))
	{
		free_world(world);
		return (1);
	}
	world->camera = camera(800, 400, PI / 3);
	if (!world->camera)
	{
		free_world(world);
		return (1);
	}
	if (parse_input("test.rt", world))
	{
		free_world(world);
		return (1);
	}
	world->canvas->lightsource.position = set_vector(-10, 3, -10, 0);
	image = render_image(world->camera, world);
	if (!image)
	{
		free_world(world);
		return (1);
	}
	mlx_image_to_window(world->canvas->mlx_ptr, image, 0, 0);
	mlx_loop(world->canvas->mlx_ptr);
	free_world(world);
	return (0);
}
