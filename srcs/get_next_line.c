/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouatt <sobouatt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 22:31:26 by sobouatt          #+#    #+#             */
/*   Updated: 2022/03/10 13:34:35 by sobouatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_end(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

int	where(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (0);
}

void	save_space(char **save, char **line)
{
	if (check_end(*save) == 1)
	{
		*line = ft_substr(*save, 0, where(*save), 0);
		*save = ft_substr(*save, where(*save) + 1, ft_strlen(*save), 1);
	}
	else
	{
		*line = ft_substr((*save), 0, ft_strlen(*save), 1);
		*save = NULL;
	}
}

int	get_next_line(int fd, char **line)
{
	static char	*save;
	char		*buffer;
	int			read_rt;

	read_rt = 1;
	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (-1);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (-1);
	while (check_end(save) != 1 && read_rt != 0)
	{
		read_rt = read(fd, buffer, BUFFER_SIZE);
		if (read_rt == -1)
		{
			free(buffer);
			return (-1);
		}
		buffer[read_rt] = '\0';
		save = ft_strjoin(save, buffer);
	}
	save_space(&save, line);
	free(buffer);
	return (!(read_rt == 0 && save == NULL));
}
