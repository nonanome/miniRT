/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_world.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 22:08:08 by qhahn             #+#    #+#             */
/*   Updated: 2025/02/05 16:05:21 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "world.h"

t_ray	ray_for_pixel(t_camera cam, int px, int py)
{
	double	xoffset;
	double	yoffset;
	double	world_x;
	double	world_y;
	double	pixel_x;
	double	pixel_y;
	t_ray	ray;

	xoffset = (px + 0.5) * cam.pixel_size;
	yoffset = (py + 0.5) * cam.pixel_size;
	world_x = cam.half_width - xoffset;
	world_y = cam.half_height - yoffset;
	pixel_x = cam.transform[0][0] * world_x + cam.transform[1][0] * world_y
		- cam.transform[2][0];
	pixel_y = cam.transform[0][1] * world_x + cam.transform[1][1] * world_y
		- cam.transform[2][1];
	ray.origin = set_vector(0, 0, 0, 1);
	ray.direction = normalize(set_vector(pixel_x, pixel_y, -1, 0));
	return (ray);
}

t_camera	camera(int hsize, int vsize, double field_of_view)
{
	t_camera	cam;
	double		half_view;
	double		aspect;

	cam.hsize = hsize;
	cam.vsize = vsize;
	cam.field_of_view = field_of_view;
	cam.transform = get_identity_matrix();
	half_view = tan(cam.field_of_view / 2);
	aspect = (double)cam.hsize / (double)cam.vsize;
	if (aspect >= 1)
	{
		cam.half_width = half_view;
		cam.half_height = half_view / aspect;
	}
	else
	{
		cam.half_width = half_view * aspect;
		cam.half_height = half_view;
	}
	cam.pixel_size = (cam.half_width * 2) / cam.hsize;
	return (cam);
}

double	**matrix(double lx, double ly, double lz, double lw, double ux,
		double uy, double uz, double uw, double fx, double fy, double fz,
		double fw, double tx, double ty, double tz, double tw)
{
	double	**mat;
	int		i;

	mat = (double **)malloc(4 * sizeof(double *));
	if (!mat)
		return (NULL);
	i = 0;
	while (i < 4)
	{
		mat[i] = (double *)malloc(4 * sizeof(double));
		if (!mat[i])
			return (NULL);
		i++;
	}
	mat[0][0] = lx;
	mat[0][1] = ly;
	mat[0][2] = lz;
	mat[0][3] = lw;
	mat[1][0] = ux;
	mat[1][1] = uy;
	mat[1][2] = uz;
	mat[1][3] = uw;
	mat[2][0] = fx;
	mat[2][1] = fy;
	mat[2][2] = fz;
	mat[2][3] = fw;
	mat[3][0] = tx;
	mat[3][1] = ty;
	mat[3][2] = tz;
	mat[3][3] = tw;
	return (mat);
}

double	**view_transform(xyzvektor from, xyzvektor to, xyzvektor up)
{
	xyzvektor forward = normalize(substraction(to, from));
	xyzvektor left = crossProduct(forward, normalize(up));
	xyzvektor true_up = crossProduct(left, forward);

	double **orientation = matrix(left.x, left.y, left.z, 0, true_up.x,
			true_up.y, true_up.z, 0, -forward.x, -forward.y, -forward.z, 0, 0,
			0, 0, 1);
	double **translation_mat = translation(-from.x, -from.y, -from.z);

	return (multiply_matrix(orientation, translation_mat));
}