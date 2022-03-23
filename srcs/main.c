#include "cub3d.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n -1)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int   ft_check_file_name(char *str)
{
  int i;

  i = 0;
  while (str[i])
    i++;
  i -= 4;
  if (i <= 0)
    return (1);
  if (ft_strcmp(str + i, ".cub") == 0)
    return (0);
  return (i);

}

char	*ft_strjoin2(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	if (s1)
	{
		while (s1[i] && s1)
		{
			str[i] = s1[i];
			i++;
		}
	}
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	if (s1)
    	free(s1);
	return (str);
}

char  **ft_store_map(int fd)
{
    int gnl_rt;
    char  *tmp_map;
    char **map;
    char *line;
  
    gnl_rt = 1;
    tmp_map = NULL;
    while (gnl_rt)
    {
      gnl_rt = get_next_line(fd, &line);
      tmp_map = ft_strjoin2(tmp_map, line);
      tmp_map = ft_strjoin2(tmp_map, "\n");
	  free(line);
    }
    map = ft_split(tmp_map, '\n');
	free(tmp_map);
    return (map);
}

int	check_around(char **map, int i, int j)
{
	if (i == 0 || j == 0)
		return (1);
	if (ft_strlen(map[i]) - 1 == j)
		return (1);
	if (map[i + 1] == NULL)
		return (1);
	if (ft_strlen(map[i - 1]) - 1 < j || ft_strlen(map[i + 1]) - 1 < j)
		return (1);
	if (map[i + 1][j] == ' ' || map[i - 1][j] == ' '|| map[i][j - 1] == ' ' || map[i][j + 1] == ' ')
		return (1);
	return (0);
}

int	check_open_map(char **map)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while(map[i] != NULL)
	{
		j = 0;
		while (map[i][j] != '\0')
		{
			if (map[i][j] != ' ' && map[i][j] != '1')
				if (check_around(map, i, j) != 0)	
					return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	is_charset(char c)
{
	if (c == '0' || c == '1' || c == ' ' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

int	check_chars(char **map, int *x, int *y)
{
	int p_count;
	int i;
	int j;

	i = 0;
	j = 0;
	p_count = 0;
	if (map == NULL)
		return (1);
	while (map[i] != NULL)
	{
		j = 0;
		while (map[i][j] != '\0')
		{
			if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E' || map[i][j] == 'W')
			{
				p_count++;
				*x = j;
				*y = i;
			}
			else if (is_charset(map[i][j]) != 1)
				return (printf("Error\nInvalid character %c at the position [%d][%d]", map[i][j], i, j));
			j++;
		}
		i++;
	}
	if (p_count != 1)
		return (printf("Error\nNo player on the map\n"));
	return (0);
}

int	ft_check_map(t_cub *cub)
{
	int x;
	int y;

	x = -1;
	y = -1;
	if (check_chars(cub->map + 6, &x, &y) != 0)
		return (1);
	if (check_open_map(cub->map + 6) != 0)
		return (printf("Error\nMap is not closed\n"));
	if (cub->map[y + 6][x] == 'N')
	{
		cub->player.dir_x = 0;
		cub->player.dir_y = -1;
		cub->player.plane_x = 0.66;
		cub->player.plane_y = 0;
	}
	if (cub->map[y + 6][x] == 'S')
	{
		cub->player.dir_x = 0;
		cub->player.dir_y = 1;
		cub->player.plane_x = -0.66;
		cub->player.plane_y = 0;
	}
	if (cub->map[y + 6][x] == 'W')
	{
		cub->player.dir_x = -1;
		cub->player.dir_y = 0;
		cub->player.plane_x = 0;
		cub->player.plane_y = -0.66;
	}
	if (cub->map[y + 6][x] == 'E')
	{
		cub->player.dir_x = 1;
		cub->player.dir_y = 0;
		cub->player.plane_x = 0;
		cub->player.plane_y = 0.66;
	}
	return (0);		
}

int	ft_free_map(char **map)
{
	int i;
	
	i = 0;
	while (map[i] != NULL)
		free(map[i++]);
	free(map);
	return (1);
}

void	display_map(char **map)
{
	int i;

	i = 0;
	while (map[i] != NULL)
		printf("%s\n", map[i++]);
}

char	**store_map(int ac, char **av)
{
	int   fd;
	
	fd = 0;
	if (ac != 2)
	{
		printf("Error\nInvalid number of arguments\n");
		return (NULL);
	}
	if (ft_check_file_name(av[1]) != 0)
	{
		printf("Error\nNot a .cub file\n");
		return (NULL);
	}
	fd = open(av[1], O_RDONLY);
	if (fd < 1)
	{
		printf("Error\nCouldn't open file\n");
		return (NULL);
	}
	return (ft_store_map(fd));
}

int	ft_skip_spaces(char *str, int i)
{
	while (str[i] == ' ')
		i++;
	return (i);
}

int	check_spaces(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

int	free_textures(t_cub *cub)
{
	if (cub->path_to_north != NULL)
		free(cub->path_to_north);
	if (cub->path_to_south != NULL)
		free(cub->path_to_south);
	if (cub->path_to_west != NULL)
		free(cub->path_to_west);
	if (cub->path_to_east != NULL)
		free(cub->path_to_east);
	return (1);
}

int	get_texture(t_cub *cub, char *str)
{
	int i;

	i = 0;
	while (str[i] == ' ')
		i++;
	if (ft_strncmp(str, "NO", 2) == 0)
	{
		i += ft_skip_spaces(str + i, 2);
		cub->path_to_north = ft_strdup(str + i);
	}
	else if (ft_strncmp(str, "SO", 2) == 0)
	{
		i += ft_skip_spaces(str + i, 2);
		cub->path_to_south = ft_strdup(str + i);
	}
	else if (ft_strncmp(str, "WE", 2) == 0)
	{
		i += ft_skip_spaces(str + i, 2);
		cub->path_to_west = ft_strdup(str + i);
	}
	else if (ft_strncmp(str, "EA", 2) == 0)
	{
		i += ft_skip_spaces(str + i, 2);
		cub->path_to_east = ft_strdup(str + i);
	}
	else
		return (0);
	if (check_spaces(str + i))
		return (1);
	return (0);
}

int	get_color(char *str, char **C, char **F)
{
	int i;

	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == 'C')
	{
		i += ft_skip_spaces(str + i, 1);
		*C = ft_strdup(str + i);
	}
	else if (str[i] == 'F')
	{
		i += ft_skip_spaces(str + i, 1);
		*F = ft_strdup(str + i);
	}
	return (0);
}

int	fill_txt(t_cub *cub)
{
	int i;
	
	i = 0;
	while (cub->map[i] != NULL)
	{
		if (get_texture(cub, cub->map[i]) != 0)
			break;
		i++;
	}
	if (cub->path_to_north == NULL || cub->path_to_south == NULL || cub->path_to_west == NULL
			|| cub->path_to_east == NULL)
			return (printf("Error\nProblem with some of the texture file\n"));
	return (0);
}

int		ft_is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
unsigned int	get_hex(char *color)
{
	unsigned int rgb;
	unsigned int r;
	unsigned int g;
	unsigned int b;

	int i;
	i = 0;
	r = 0;
	g = 0;
	b = 0;
	while (ft_is_digit(color[i]))
	{
		if (r != 0)
			r *= 10;
		r += color[i++] - '0';
	}
	if (color[i++] != ',' || r >= 255)
		return (printf("Salut1\n"));
	while (ft_is_digit(color[i]))
	{
		if (g != 0)
			g *= 10;
		g += color[i++] - '0';
	}
	if (color[i++] != ',' || r >= 255)
		return (printf("Salut2\n"));
	while (ft_is_digit(color[i]))
	{
		if (b != 0)
			b *= 10;
		b += color[i++] - '0';

	}
	rgb = 65536 * r + 256 * g + b;
	return (rgb);
}

void	free_either(void *F, void *C)
{
	if (F != NULL)
		free(F);
	if (C != NULL)
		free(C);
}
int	fill_colors(t_cub *cub)
{
	int i;
	char *F;
	char *C;

	F = NULL;
	C = NULL;
	i = 0;
	while (cub->map[i] != NULL)
		if (get_color(cub->map[i++], &F, &C) != 0)
			break;
	if (F == NULL || C == NULL)
	{
		free_either(F, C);
		return (printf("Error\nProblen with some with floor or ceiling color\n"));
	}
	cub->F = get_hex(F);
	cub->C = get_hex(C);
	if (C == 0 || F == 0)
		printf("Problem\n");
	free(F);
	free(C);
	return (0);
}

int	fill_textures(t_cub *cub)
{
	cub->path_to_north = NULL;
	cub->path_to_south = NULL;
	cub->path_to_west = NULL;
	cub->path_to_east = NULL;
	if (fill_txt(cub) != 0)
		return (free_textures(cub));
	if (fill_colors(cub) != 0)
		return (free_textures(cub));
	return (0);
}

int	main(int ac, char **av)
{
	t_cub cub;

	cub.map = store_map(ac, av);
	if (cub.map == NULL)
		return (0);
	if (fill_textures(&cub) != 0)
		return (ft_free_map(cub.map));
	if (ft_check_map(&cub) != 0)
	{
		free_textures(&cub);
		return (ft_free_map(cub.map));
	}
	free_textures(&cub);
	ft_free_map(cub.map);
}
