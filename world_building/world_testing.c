#include "world.h"
#include "../miniRT.h"
#include <stdlib.h>
#include <stdio.h>

t_material set_material(xyzvektor color, double ambient, double diffuse, double specular, double shininess)
{
	t_material material;

	material.color = get_color_from_tuple(color);
	material.ambient = ambient;
	material.diffuse = diffuse;
	material.specular = specular;
	material.shininess = shininess;
	return (material);
}

int make_test_world(t_world *world)
{
	xyzvektor color;

	color = set_vector(1.0, 0.0, 0.0, 0.0);
	if (!world)
		return (1);
	init_canvas(world->canvas);
	world->env->wind = set_vector(0.0, 0.0, 0.0, 0.0);
	world->env->gravity = set_vector(0.0, 0.0, 0.0, 0.0);
	world->spheres[0].id = 0;
	world->spheres[0].origin = set_vector(0.0, 0.0, 0.0, 1.0);
	world->spheres[0].radius = 1.0;
	world->spheres[0].default_transformation = get_identity_matrix();
	world->spheres[0].material = set_material(color , 0.2, 0.7, 0.2, 200.0);
	world->spheres[1].id = 1;
	world->spheres[1].origin = set_vector(0.0, 0.0, 0.0, 1.0);
	world->spheres[1].radius = 0.5;
	world->spheres[1].default_transformation = get_identity_matrix();
	world->spheres[1].material = set_material(color , 0.2, 0.9, 0.9, 200.0);
	world->nr_spheres = 2;
	world->canvas->all_intersections.nr_intersection_entries = 0;
	world->all_sorted = malloc(sizeof(double *) * 100);
	if (!world->all_sorted)
	{
		free_world(world);
		return (1);
	}
	return (0);
}

int main (void)
{
	t_world *world;
	t_ray ray;

	world = get_world(2);
	if (!world)
		return (1);
	if (make_test_world(world))
	{
		free_world(world);
		return (1);
	}
	ray = init_ray();
	intersect_world(world, ray);
	//printf("world->spheres[0].id = %d\n", world->spheres[0].id);
	//printf("world->spheres[0].origin.x = %f\n", world->spheres[0].origin.x);
	//printf("world->spheres[0].origin.y = %f\n", world->spheres[0].origin.y);
	//printf("world->spheres[0].origin.z = %f\n", world->spheres[0].origin.z);
	//printf("world->spheres[0].origin.w = %f\n", world->spheres[0].origin.w);
	//printf("world->spheres[0].radius = %f\n", world->spheres[0].radius);
	//printf("world->spheres[0].material.ambient = %f\n", world->spheres[0].material.ambient);
	//printf("world->spheres[0].material.diffuse = %f\n", world->spheres[0].material.diffuse);
	//printf("world->spheres[0].material.specular = %f\n", world->spheres[0].material.specular);
	//printf("world->spheres[0].material.shininess = %f\n", world->spheres[0].material.shininess);
	//printf("world->spheres[1].id = %d\n", world->spheres[1].id);
	//printf("world->spheres[1].origin.x = %f\n", world->spheres[1].origin.x);
	//printf("world->spheres[1].origin.y = %f\n", world->spheres[1].origin.y);
	//printf("world->spheres[1].origin.z = %f\n", world->spheres[1].origin.z);
	//printf("world->spheres[1].origin.w = %f\n", world->spheres[1].origin.w);
	//printf("world->spheres[1].radius = %f\n", world->spheres[1].radius);
	//printf("world->spheres[1].material.ambient = %f\n", world->spheres[1].material.ambient);
	//printf("world->spheres[1].material.diffuse = %f\n", world->spheres[1].material.diffuse);
	//printf("world->spheres[1].material.specular = %f\n", world->spheres[1].material.specular);
	//printf("world->spheres[1].material.shininess = %f\n", world->spheres[1].material.shininess);
	//printf("world->spheres[1].material.color = %d\n", world->spheres[1].material.color);
	int i = 0;
	while (world->all_sorted[i] != 0)
	{
		printf("world->all_sorted[%d] = %f\n", i, world->all_sorted[i]);
		i ++;
	}
	printf("world.canvas->all_intersections.nr_intersections = %ld\n", world->canvas->all_intersections.nr_intersections);
	free_world(world);
	return (0);
}
