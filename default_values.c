#include "miniRT.h"



t_material default_material(void)
{
	t_material dm;
	xyzvektor color;

	color.x = 0;
	color.y = 0;
	color.z = 1;
	color.w = 1;

	dm.color = get_color_from_tuple(color);
	dm.ambient = 0.2;
	dm.diffuse = 0.9;
	dm.specular = 0.9;
	dm.shininess = 200.0;
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