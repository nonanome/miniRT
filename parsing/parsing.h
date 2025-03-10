/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:03:30 by qhahn             #+#    #+#             */
/*   Updated: 2025/03/10 18:07:21 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../structs.h"
#include "../miniRT.h"
#include "../world_building/world.h"
#include <fcntl.h>
#include <unistd.h>

int budget_get_next_line(int fd, char **line);
char **process_file(char *file_name);
double budget_ft_atof(char *str);