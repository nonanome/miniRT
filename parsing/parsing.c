/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:54:58 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/10 18:51:50 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int budget_get_next_line(int fd, char **line)
{
    char buffer[2];
    int ret;
    char *temp;
    int i;

    i = 0;
    *line = malloc(1);
    if (!*line)
        return -1;
    (*line)[0] = '\0';
    while ((ret = read(fd, buffer, 1)) > 0)
    {
        buffer[1] = '\0';
        if (buffer[0] == '\n')
            return 1;
        temp = *line;
        *line = ft_strjoin(*line, buffer);
        free(temp);
        i++;
    }
    if (ret == 0)
        return 0;
    return -1;
}

char **process_file(char *file_name)
{
    char **input;
    int fd;
    char *line;
    int i;

    i = 0;
    fd = open(file_name, O_RDONLY);
    printf("fd: %d\n", fd);
    input = malloc(100 * sizeof(char *));
    if (!input)
        return NULL;
    while (budget_get_next_line(fd, &line) > 0)
    {
        input[i] = line;
        i++;
    }
    input[i] = NULL;
    return input;
}

void ft_free_split(char **split)
{
    int i;

    i = 0;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}

int parse_ambient_light(t_world *world, char *line)
{
    char **split;
    char **rgb_split;
    
    split = ft_split(line, ' ');
    if (!split)
        return (1);
    double ratio = budget_ft_atof(split[1]);
    if (ratio < 0 || ratio > 1)
        return (ft_free_split(split), 1);
    rgb_split = ft_split(split[2], ',');
    if (!rgb_split)
        return (ft_free_split(split), 1);
    double r = ft_atoi(rgb_split[0]) / 255.0;
    double g = ft_atoi(rgb_split[1]) / 255.0;
    double b = ft_atoi(rgb_split[2]) / 255.0;
    if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1)
        return (ft_free_split(split), ft_free_split(rgb_split), 1);
    *(world->ambient) = set_vector(r, g, b, 0);
    world->ambient_intensity = ratio;
    ft_free_split(rgb_split);
    ft_free_split(split);
    return (0);
}

int parse_xyz(char **split, double *xyz, int mode)
{
    int i;

    i = 0;
    while (split[i])
    {
        xyz[i] = budget_ft_atof(split[i]);
        if (mode == 1 && (xyz[i] < -1 || xyz[i] > 1))
            return (0);
        i++;
    }
    return (1);
}

int parse_camera(t_world *world, char *line)
{
    char **split;
    char **xyz_split;
    char **normal_split;
    double xyz[3];
    double normal[3];

    split = ft_split(line, ' ');
    if (!split)
        return (1);
    if (budget_ft_atof(split[3]) < 0 || budget_ft_atof(split[3]) > 180)
        return (ft_free_split(split), 1);
    xyz_split = ft_split(split[1], ',');
    if (!xyz_split)
        return (ft_free_split(split), 1);
    normal_split = ft_split(split[2], ',');
    if (!normal_split)
        return (ft_free_split(split), ft_free_split(xyz_split), 1);
    if (!parse_xyz(xyz_split, xyz, 0))
        return (ft_free_split(split), ft_free_split(xyz_split), ft_free_split(normal_split), 1);
    if (!parse_xyz(normal_split, normal, 1))
        return (ft_free_split(split), ft_free_split(xyz_split), ft_free_split(normal_split), 1);
    if (normal[0] < -1 || normal[0] > 1 || normal[1] < -1 || normal[1] > 1 || normal[2] < -1 || normal[2] > 1)
        return (ft_free_split(split), ft_free_split(xyz_split), ft_free_split(normal_split), 1);
    world->camera->field_of_view = budget_ft_atof(split[3]);
    world->camera->transform = view_transform(set_vector(xyz[0], xyz[1], xyz[2], 1), set_vector(normal[0], normal[1], normal[2], 0), set_vector(0, 1, 0, 0));
    return (ft_free_split(split), ft_free_split(xyz_split), 0);
}

int parse_input(char *file_name, t_world *world)
{
    char **input;
    int i;

    i = 0;
    input = process_file(file_name);
    while (input[i])
    {
        if (input[i][0] == 'A')
            parse_ambient_light(world, input[i]);
        else if (input[i][0] == 'C')
            parse_camera(world, input[i]);
        // else if (input[i][0] == 'L')
        //     parse_light(world, input[i]);
        // else if (ft_strncmp(input[i], "sp", 2) == 0)
        //     parse_sphere(world, input[i]);
        // else if (ft_strncmp(input[i], "pl", 2) == 0)
        //     parse_plane(world, input[i]);
        i++;
    }
    return (0);
}
