/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:03:30 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/22 17:30:17 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../structs.h"
#include "../world_building/world.h"
#include <fcntl.h>
#include <unistd.h>

int		budget_get_next_line(int fd, char **line);
char	**process_file(char *file_name);
double	budget_ft_atof(char *str);
int		budget_get_next_line(int fd, char **line);
int		parse_light(t_world *world, char *line);
int		parse_camera(t_world *world, char *line);
int		parse_ambient_light(t_world *world, char *line);
int		parse_sphere(t_world *world, char *line);
int		parse_plane(t_world *world, char *line);
int		parse_cylinder(t_world *world, char *line);
void	ft_free_split(char **split);
int		parse_xyz(char **split, double *xyz, int mode);
int		parse_common_shape(char **split, double pos[3], double rgb[3]);

void	check_camera_line(char *line);
void	check_spere_or_light_line(char *line);
void	check_plane_line(char *line);
void	check_cylinder_line(char *line);
void	check_ambient_light_line(char *line);
int		parse_rgb(char *str, double rgb[3]);
int		parse_ratio(char *str, double *ratio);