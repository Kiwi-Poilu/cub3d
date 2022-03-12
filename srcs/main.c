#include "cub3d.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
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

int	flood_fill(char **map, int x, int y)
{
	printf("map[y][x] = %c\n", map[y][x]);
	printf("y=%d x=%d\n", y , x);
	if (map[y] == NULL)
	{
		printf("map[y]==NULL\n");
		return (1);
	}
	if (map[y][x] == ' ')
	{
		printf("map[y][x] == ' '");
		return (1);
	}
	if (map[y][x] == '1')
		return (0);
	if (x == 0 || y == 0 || map[y][x] == '\n')
	{
		printf("x == %d || y == %d || ft_strlen(map[y] = %d\n", x, y, ft_strlen(map[y]));
		return (1);
	}
	if (flood_fill(map, x - 1, y) == 1)
		return (1);
	if (flood_fill(map, x + 1, y) == 1)
		return (1);
	if (flood_fill(map, x, y - 1) == 1)
		return (1);
	if (flood_fill(map, x, y + 1) == 1)
		return (1);
	return (0);
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
		return (1);
	return (0);
}

int	ft_check_map(t_cub *cub)
{
	int x;
	int y;

	x = -1;
	y = -1;
	if (check_chars(cub->map, &x, &y) != 0)
		return (1);
	if (check_open_map(cub->map) != 0)
		return (printf("Error\nMap is not closed\n"));
	cub->player.map_x = x;
	cub->player.map_y = y;
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

int	main(int ac, char **av)
{
	t_cub cub;

	cub.map = store_map(ac, av);
	if (cub.map == NULL)
		return (0);
	display_map(cub.map);
	if (ft_check_map(&cub) != 0)
		return (ft_free_map(cub.map));
	ft_free_map(cub.map);
}
