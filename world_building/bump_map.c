/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:16:03 by kkuhn             #+#    #+#             */
/*   Updated: 2025/04/09 17:21:10 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

int	get_bumpmap_grayscale(mlx_texture_t *texture, int x, int y)
{
	uint32_t	*pixels;
	uint32_t	color;
	int			gray;
	uint8_t		rgb[3];

	if (!texture || x < 0 || y < 0 || x >= texture->width
		|| y >= texture->height)
		return (-1);
	pixels = (uint32_t *)texture->pixels;
	color = pixels[y * texture->width + x];
	rgb[0] = (color >> 0) & 0xFF;
	rgb[1] = (color >> 8) & 0xFF;
	rgb[2] = (color >> 16) & 0xFF;
	gray = (int)(0.299 * rgb[0] + 0.587 * rgb[1] + 0.114 * rgb[2]);
	if (gray < 0)
		gray = 0;
	if (gray > 255)
		gray = 255;
	return (gray);
}

void	load_bumpmap(char *path, t_world *world)
{
	mlx_texture_t	*texture;

	texture = mlx_load_png(path);
	if (!texture)
	{
		printf("Fehler beim Laden der Bumpmap: %s\n", path);
		return ;
	}
	world->canvas->bumpmapcolor = mlx_texture_to_image(world->canvas->mlx_ptr,
			texture);
	world->canvas->bumpmap = texture;
}
