#include "cub3d.h"
#include <string.h>
#include <stdlib.h>

void  ft_init(t_cub *cub)
{
  cub->player.pos_x = 3; //
  cub->player.pos_y = 3; //
  cub->player.dir_x = -1; 
  cub->player.dir_y = 0;
  cub->player.plane_x = 0;
  cub->player.plane_y = 0.66;
  cub->player.time = 0;
  cub->player.old_time = 0;
  cub->mlx.y = 0;
}

void  ft_parse(t_cub *cub)
{
  int i;

  i = 0;
  cub->map.tab = (char **)malloc(sizeof(char *) * 100);
  while (i < 100)
  {
    cub->map.tab[i] = (char *)malloc(sizeof(char) * 100);
    i++;
  }
  cub->map.tab[0] = ft_strcpy(cub->map.tab[0], "111111111111111");
  cub->map.tab[1] = ft_strcpy(cub->map.tab[1], "100000010000001");
  cub->map.tab[2] = ft_strcpy(cub->map.tab[2], "100000000000001");
  cub->map.tab[3] = ft_strcpy(cub->map.tab[3], "101000000000011");
  cub->map.tab[4] = ft_strcpy(cub->map.tab[4], "111111111111111");
  cub->map.height = 5; //
  cub->map.width = 15; //
  cub->mlx.ceiling_color = 0x0000FF;
  cub->mlx.floor_color = 0xFF0000;
}

void  ft_put_font(t_cub *cub)
{
  int  tmp;

  if (cub->mlx.pixel_bits != 32)
  {
    cub->mlx.floor_color = mlx_get_color_value(cub->mlx.ptr, cub->mlx.floor_color);
    cub->mlx.ceiling_color = mlx_get_color_value(cub->mlx.ptr, cub->mlx.ceiling_color);
  }
  tmp = cub->mlx.ceiling_color;
  while (cub->mlx.y < WIN_HEIGHT)
  {
    cub->mlx.x = 0;
    if (cub->mlx.y == WIN_HEIGHT / 2)
      tmp = cub->mlx.floor_color;
    while (cub->mlx.x < WIN_WIDTH)
    {
      cub->mlx.pixel_bits = (cub->mlx.y * cub->mlx.line_bytes) + (cub->mlx.x * 4);
      if (cub->mlx.endian == 1)
      {
        cub->mlx.buffer[cub->mlx.pixel_bits] = tmp >> 24;
        cub->mlx.buffer[cub->mlx.pixel_bits + 1] = (tmp >> 16) & 0xFF;
        cub->mlx.buffer[cub->mlx.pixel_bits + 2] = (tmp >> 8) & 0xFF;
        cub->mlx.buffer[cub->mlx.pixel_bits + 3] = tmp & 0xFF;
      }
      else if (cub->mlx.endian == 0)
      {
        cub->mlx.buffer[cub->mlx.pixel_bits] = tmp & 0xFF;
        cub->mlx.buffer[cub->mlx.pixel_bits + 1] = (tmp >> 8) & 0xFF;
        cub->mlx.buffer[cub->mlx.pixel_bits + 2] = (tmp >> 16) & 0xFF;
        cub->mlx.buffer[cub->mlx.pixel_bits + 3] = tmp >> 24;
      }
      cub->mlx.x++;
    }
    cub->mlx.y++;
  }
}

void  ft_put_line(t_cub *cub)
{
  int i;

  i = cub->mlx.draw_start;
  while (i < cub->mlx.draw_end)
  {
    cub->mlx.pixel_bits = (i * cub->mlx.line_bytes) + (cub->mlx.x * 4);
    if (cub->mlx.endian == 1)
    {
      cub->mlx.buffer[cub->mlx.pixel_bits] = cub->mlx.color >> 24;
      cub->mlx.buffer[cub->mlx.pixel_bits + 1] = (cub->mlx.color >> 16) & 0xFF;
      cub->mlx.buffer[cub->mlx.pixel_bits + 2] = (cub->mlx.color >> 8) & 0xFF;
      cub->mlx.buffer[cub->mlx.pixel_bits + 3] = cub->mlx.color & 0xFF;
    }
    else if (cub->mlx.endian == 0)
    {
      cub->mlx.buffer[cub->mlx.pixel_bits] = cub->mlx.color & 0xFF;
      cub->mlx.buffer[cub->mlx.pixel_bits + 1] = (cub->mlx.color >> 8) & 0xFF;
      cub->mlx.buffer[cub->mlx.pixel_bits + 2] = (cub->mlx.color >> 16) & 0xFF;
      cub->mlx.buffer[cub->mlx.pixel_bits + 3] = cub->mlx.color >> 24;
    }
    i++;
  }
}

void  ft_put_wall(t_cub *cub)
{
  while (cub->mlx.x < WIN_WIDTH)
  {
    cub->player.camera_x = 2 * cub->mlx.x / (double)WIN_WIDTH - 1;
    cub->player.ray_x = cub->player.dir_x + cub->player.plane_x * cub->player.camera_x;
    cub->player.ray_y = cub->player.dir_y + cub->player.plane_y * cub->player.camera_x;
    cub->player.map_x = (int)(cub->player.pos_x);
    cub->player.map_y = (int)(cub->player.pos_y);
    if (cub->player.ray_x == 0)
      cub->player.delta_x = 1e30;
    else
      cub->player.delta_x = fabs(1 / cub->player.ray_x);
    if (cub->player.ray_y == 0)
      cub->player.delta_y = 1e30;
    else
      cub->player.delta_y = fabs(1 / cub->player.ray_y);
    cub->player.hit = 0;
    if (cub->player.ray_x < 0)
    {
      cub->player.step_x = -1;
      cub->player.side_x = (cub->player.pos_x - cub->player.map_x) * cub->player.delta_x;
    }
    else
    {
      cub->player.step_x = 1;
      cub->player.side_x = (cub->player.map_x + 1.0 - cub->player.pos_x) * cub->player.delta_x;
    }
    if (cub->player.ray_y < 0)
    {
      cub->player.step_y = -1;
      cub->player.side_y = (cub->player.pos_y - cub->player.map_y) * cub->player.delta_y;
    }
    else
    {
      cub->player.step_y = 1;
      cub->player.side_y = (cub->player.map_y + 1.0 - cub->player.pos_y) * cub->player.delta_y;
    }
    while (cub->player.hit == 0)
    {
      if (cub->player.side_x < cub->player.side_y)
      {
        cub->player.side_x += cub->player.delta_x;
        cub->player.map_x += cub->player.step_x;
        cub->player.side = 0;
      }
      else
      {
        cub->player.side_y += cub->player.delta_y;
        cub->player.map_y += cub->player.step_y;
        cub->player.side = 1;
      }
      if (cub->map.tab[cub->player.map_x][cub->player.map_y] != '0')
        cub->player.hit = 1;
    }
    if (cub->player.side == 0)
      cub->player.wall = (cub->player.side_x - cub->player.delta_x);
    else
      cub->player.wall = (cub->player.side_y - cub->player.delta_y);
    cub->mlx.line_height = (int)(WIN_HEIGHT / cub->player.wall);
    cub->mlx.draw_start = -cub->mlx.line_height / 2 + WIN_HEIGHT / 2;
    if (cub->mlx.draw_start < 0)
      cub->mlx.draw_start = 0;
    cub->mlx.draw_end = cub->mlx.line_height / 2 + WIN_HEIGHT / 2;
    if (cub->mlx.draw_end >= WIN_HEIGHT)
      cub->mlx.draw_end = WIN_HEIGHT - 1;
    cub->mlx.color = 0x00FF00;
    if (cub->player.side == 1)
      cub->mlx.color = cub->mlx.color / 2;
    ft_put_line(cub);
    cub->mlx.x++;
  }
}

int  ft_key(int code, t_cub *cub)
{
  double  tmp;

  if (code == UP)
  {
    if (cub->map.tab[(int)(cub->player.pos_x + cub->player.dir_x * cub->player.move_speed)][(int)cub->player.pos_x] == '0')
      cub->player.pos_x += cub->player.dir_x * cub->player.move_speed;
    if (cub->map.tab[(int)cub->player.pos_x][(int)(cub->player.pos_y + cub->player.dir_y * cub->player.move_speed)] == '0')
      cub->player.pos_y += cub->player.dir_y * cub->player.move_speed;
  }
  else if (code == LEFT)
  {
    tmp = cub->player.dir_x;
    cub->player.dir_x = cub->player.dir_x * cos(cub->player.rot_speed) - cub->player.dir_y * sin(cub->player.rot_speed);
    cub->player.dir_y = tmp * sin(cub->player.rot_speed) + cub->player.dir_y * cos(cub->player.rot_speed);
    tmp = cub->player.plane_x;
    cub->player.plane_x = cub->player.plane_x * cos(cub->player.rot_speed) - cub->player.plane_y * sin(cub->player.rot_speed);
    cub->player.plane_y = tmp * sin(cub->player.rot_speed) + cub->player.plane_y * cos(cub->player.rot_speed);
  }
  else if (code == DOWN)
  {
    if (cub->map.tab[(int)(cub->player.pos_x - cub->player.dir_x * cub->player.move_speed)][(int)cub->player.pos_y] == '0')
      cub->player.pos_x -= cub->player.dir_x * cub->player.move_speed;
    if (cub->map.tab[(int)cub->player.pos_x][(int)(cub->player.pos_y - cub->player.dir_y * cub->player.move_speed)] == '0')
      cub->player.pos_y -= cub->player.dir_y * cub->player.move_speed;
  }
  else if (code == RIGHT)
  {
    tmp = cub->player.dir_x;
    cub->player.dir_x = cub->player.dir_x * cos(-cub->player.rot_speed) - cub->player.dir_y * sin(-cub->player.rot_speed);
    cub->player.dir_y = tmp * sin(-cub->player.rot_speed) + cub->player.dir_y * cos(-cub->player.rot_speed);
    tmp = cub->player.plane_x;
    cub->player.plane_x = cub->player.plane_x * cos(-cub->player.rot_speed) - cub->player.plane_y * sin(-cub->player.rot_speed);
    cub->player.plane_y = tmp * sin(-cub->player.rot_speed) + cub->player.plane_y * cos(-cub->player.rot_speed);
  }
  return (0);
}

void  ft_time(t_cub *cub)
{
  cub->player.move_speed = 0.1;
  cub->player.rot_speed = 0.06;
}

int   ft_frame(t_cub *cub)
{
  cub->mlx.image = mlx_new_image(cub->mlx.ptr, WIN_WIDTH, WIN_HEIGHT);
  cub->mlx.buffer = mlx_get_data_addr(cub->mlx.image, &(cub->mlx.pixel_bits), &(cub->mlx.line_bytes), &(cub->mlx.endian));
  ft_put_font(cub);
  cub->mlx.x = 0;
  cub->mlx.y = 0;
  ft_put_wall(cub);
  ft_time(cub);
  mlx_put_image_to_window(cub->mlx.ptr, cub->mlx.win, cub->mlx.image, 0, 0);
  mlx_destroy_image (cub->mlx.ptr, cub->mlx.image);
  return (0);
}


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

void  ft_cub3d(t_cub *cub)
{
  mlx_loop_hook(cub->mlx.ptr, ft_frame, cub);
  mlx_hook(cub->mlx.win, 2, 1L<<0, ft_key, cub);
  mlx_loop(cub->mlx.ptr);
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
	{
		printf("i == 0\n");
		return (1);
	}
	if (ft_strlen(map[i]) - 1 == j)
	{
		printf("strlen = j");
		return (1);
	}
	if (map[i + 1] == NULL)
	{	
		printf("map i + 1 == NULL\n");
		return (1);
	}
	if (ft_strlen(map[i - 1]) - 1 < j || ft_strlen(map[i + 1]) - 1 < j)
	{
		printf("ft_strlen(map[i - 1]) - 1=%d ft_strlen(map[i + 1]) - 1=%d i=%d j=%d", ft_strlen(map[i - 1]) - 1
		, ft_strlen(map[i + 1]) - 1, i, j);
		return (1);
	}
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

int	ft_check_map(char **map)
{
	int x;
	int y;

	x = -1;
	y = -1;
	if (check_chars(map, &x, &y) != 0)
		return (1);
	if (check_open_map(map) != 0)
		return (printf("Error\nMap is not closed\n"));
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

int	main(int ac, char **av)
{
	int   fd;
	//t_cub cub;
	char **map;

	fd = 0;
	if (ac != 2)
	  return (printf("Error\nInvalid number of arguments\n"));
	if (ft_check_file_name(av[1]) != 0)
		return (printf("Error\nNot a .cub file\n"));
	fd = open(av[1], O_RDONLY);
	if (fd < 1)
		return (printf("Error\nCouldn't open file\n"));
	map = ft_store_map(fd);
	if (ft_check_map(map) != 0)
		return (ft_free_map(map));
	ft_free_map(map);
}
