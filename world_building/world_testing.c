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
	// // Set up the default world
	// world->canvas->lightsource.color = set_vector(1.0, 1.0, 1.0, 1.0);
	// world->canvas->lightsource.position = set_vector(-10, 10, -10, 1);

	// // Sphere 1
	// world->shapes[0] = new_sphere();
	// world->shapes[0].material = set_material(set_vector(0.8, 1.0, 0.6, 1.0), 0.1, 0.7, 0.2, 200.0);

	// // Sphere 2
	// world->shapes[1] = new_sphere();
	// world->shapes[1].default_transformation = scaling(0.5, 0.5, 0.5);

	// // Set the number of shapes
	// world->nr_shapes = 2;
	//floor
	world->shapes[0] = new_shape(0);
	world->shapes[0].default_transformation = scaling(10, 0.01, 10);
	world->shapes[0].material = set_material(color4, 0.1, 0.7, 0.1, 200.0);
	//left wall
	world->shapes[1] = new_shape(0);
	world->shapes[1].default_transformation = multiply_matrix(
			multiply_matrix(translation(0, 0, 5), rotation_y(-45)),
			multiply_matrix(rotation_x(90), scaling(10, 0.01, 10)));
	world->shapes[1].material = set_material(color4, 0.1, 0.7, 0.1, 200.0);
	//right wall
	world->shapes[2] = new_shape(0);
	world->shapes[2].default_transformation = multiply_matrix(
			multiply_matrix(translation(0, 0, 5), rotation_y(45)),
			multiply_matrix(rotation_x(90), scaling(10, 0.01, 10)));
	world->shapes[2].material = set_material(color4, 0.1, 0.7, 0.1, 200.0);
	//middle
	world->shapes[3] = new_shape(0);
	world->shapes[3].default_transformation = translation(-0.5, 1, 0.5);
	world->shapes[3].material = set_material(color2, 0.1, 0.7, 0.3, 200.0);
	//right
	world->shapes[4] = new_shape(0);
	world->shapes[4].default_transformation = multiply_matrix(
			translation(1.5, 0.5, -0.5), scaling(0.5, 0.5, 0.5));
	world->shapes[4].material = set_material(color, 0.1, 0.7, 0.3, 200.0);
	//left
	world->shapes[5] = new_shape(0);
	world->shapes[5].default_transformation = multiply_matrix(
			translation(-1.5, 0.33, -0.75), scaling(0.33, 0.33, 0.33));
	world->shapes[5].material = set_material(color3, 0.1, 0.7, 0.3, 200.0);
	world->nr_shapes = 6;
	world->canvas->lightsource.color = set_vector(1.0, 1.0, 1.0, 1.0);
	world->canvas->lightsource.position = set_vector(-10, 10, -10, 0);

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
	t_camera		cam;
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
	cam = camera(300, 150, PI / 3);
	cam.transform = view_transform(set_vector(0, 1.5, -5, 1),
			set_vector(0, 1, 0, 1), set_vector(0, 1, 0, 1));
	world->canvas->lightsource.position = set_vector(10, 10, -10, 1);
	image = render_image(cam, world);
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

// tests from intersect_world.c
// printf("ray.origin.x = %f\n", ray.origin.x);
// printf("ray.origin.y = %f\n", ray.origin.y);
// printf("ray.origin.z = %f\n", ray.origin.z);
// printf("ray.origin.w = %f\n", ray.origin.w);
// printf("ray.direction.x = %f\n", ray.direction.x);
// printf("ray.direction.y = %f\n", ray.direction.y);
// printf("ray.direction.z = %f\n", ray.direction.z);
// printf("ray.direction.w = %f\n", ray.direction.w);
// for (int i = 0; i < 4; i++)
//{
//	for (int j = 0; j < 4; j++)
//	{
//		if (i == 0 && j == 0)
//			printf("X-axis: ");
//		else if (i == 1 && j == 0)
//			printf("Y-axis: ");
//		else if (i == 2 && j == 0)
//			printf("Z-axis: ");
//		else if (i == 3 && j == 0)
//			printf("W-axis: ");
//		printf("%f ", retmatrix[i][j]);
//	}
//	printf("\n");
//}

// printf("comp.t = %f\n", comp.t);
// printf("comp.object->id = %d\n", comp.object->id);
// printf("comp.point.x = %f\n", comp.point.x);
// printf("comp.point.y = %f\n", comp.point.y);
// printf("comp.point.z = %f\n", comp.point.z);
// printf("comp.point.w = %f\n", comp.point.w);
// printf("comp.eyev.x = %f\n", comp.eyev.x);
// printf("comp.eyev.y = %f\n", comp.eyev.y);
// printf("comp.eyev.z = %f\n", comp.eyev.z);
// printf("comp.eyev.w = %f\n", comp.eyev.w);
// printf("comp.normalv.x = %f\n", comp.normalv.x);
// printf("comp.normalv.y = %f\n", comp.normalv.y);
// printf("comp.normalv.z = %f\n", comp.normalv.z);
// printf("comp.normalv.w = %f\n", comp.normalv.w);
// printf("comp.inside = %d\n", comp.inside);
// printf("world->canvas->all_intersections.nr_intersections = %ld\n",
//	world->canvas->all_intersections.nr_intersections);
// printf("world->canvas->all_intersections.intersections[1].times[0] = %f\n",
//	world->canvas->all_intersections.intersections[1].times[0]);
// printf("world->canvas->all_intersections.intersections[1].times[1] = %f\n",
//	world->canvas->all_intersections.intersections[1].times[1]);
// printf("world->canvas->all_intersections.intersections[0].times[0] = %f\n",
//	world->canvas->all_intersections.intersections[0].times[8]);
// printf("world->canvas->all_intersections.intersections[0].times[1] = %f\n",
//	world->canvas->all_intersections.intersections[0].times[9]);
// for (int i = 0; i < 4; i++)
//{
//	printf("world->all_sorted[%d] = %f\n", i, world->all_sorted[i]);
//}
// printf("world->shapes[0].id = %d\n", world->shapes[0].id);
// printf("world->shapes[0].origin.x = %f\n", world->shapes[0].origin.x);
// printf("world->shapes[0].origin.y = %f\n", world->shapes[0].origin.y);
// printf("world->shapes[0].origin.z = %f\n", world->shapes[0].origin.z);
// printf("world->shapes[0].origin.w = %f\n", world->shapes[0].origin.w);
// printf("world->shapes[0].radius = %f\n", world->shapes[0].radius);
// printf("world->shapes[0].material.ambient = %f\n",
//	world->shapes[0].material.ambient);
// printf("world->shapes[0].material.diffuse = %f\n",
//	world->shapes[0].material.diffuse);
// printf("world->shapes[0].material.specular = %f\n",
//	world->shapes[0].material.specular);
// printf("world->shapes[0].material.shininess = %f\n",
//	world->shapes[0].material.shininess);
// printf("world->shapes[1].id = %d\n", world->shapes[1].id);
// printf("world->shapes[1].origin.x = %f\n", world->shapes[1].origin.x);
// printf("world->shapes[1].origin.y = %f\n", world->shapes[1].origin.y);
// printf("world->shapes[1].origin.z = %f\n", world->shapes[1].origin.z);
// printf("world->shapes[1].origin.w = %f\n", world->shapes[1].origin.w);
// printf("world->shapes[1].radius = %f\n", world->shapes[1].radius);
// printf("world->shapes[1].material.ambient = %f\n",
//	world->shapes[1].material.ambient);
// printf("world->shapes[1].material.diffuse = %f\n",
//	world->shapes[1].material.diffuse);
// printf("world->shapes[1].material.specular = %f\n",
//	world->shapes[1].material.specular);
// printf("world->shapes[1].material.shininess = %f\n",
//	world->shapes[1].material.shininess);
// printf("world->shapes[1].material.color = %d\n",
//	world->shapes[1].material.color);
// int i = 0;
// while (world->all_sorted[i] != 0)
//{
//	printf("world->all_sorted[%d] = %f\n", i, world->all_sorted[i]);
//	i ++;
//}
// printf("world.canvas->all_intersections.nr_intersections = %ld\n",
//	world->canvas->all_intersections.nr_intersections);