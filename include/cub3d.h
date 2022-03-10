#ifndef CUB3D_H
# define CUB3D_H

# include "../mlx/mlx.h"
# include "../libft/include/libft.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080
# define BUFFER_SIZE 8
# define UP 65362
# define DOWN 65364
# define LEFT 65361
# define RIGHT 65363

typedef struct  s_player
{
  double  pos_x;
  double  pos_y;
  double  dir_x;
  double  dir_y;
  double  plane_x;
  double  plane_y;
  double  camera_x;
  double  camera_y;
  double  ray_x;
  double  ray_y;
  double  side_x;
  double  side_y;
  double  delta_x;
  double  delta_y;
  double  wall;
  double  time;
  double  old_time;
  double  move_speed;
  double  rot_speed;
  int     step_x;
  int     step_y;
  int     hit;
  int     side;
  int     map_x;
  int     map_y;
}               t_player;

typedef struct  s_mlx
{
  void          *ptr;
  void          *win;
  void          *image;
  int           pixel_bits;
  int           line_bytes;
  int           line_height;
  int           draw_start;
  int           draw_end;
  int           endian;
  char          *buffer;
  int           floor_color;
  int           ceiling_color;
  int           color;
  int           north_color;
  int           south_color;
  int           east_color;
  int           west_color;
  int           x;
  int           y;
}               t_mlx;

typedef struct  s_map
{
  char  **tab;
  int   height;
  int   width;
}               t_map;

typedef struct  s_cub
{
  t_mlx         mlx;
  t_map         map;
  t_player      player;
}               t_cub;

char	*ft_strjoin2(char *s1, char *s2);

// get_next_line.c || get_next_line_utils.c
int   get_next_line(int fd, char **line);
char  *ft_strjoin(char *s1, char *s2);
int   ft_strlen(char *str);
char	*ft_strdup(char *str);
char	*ft_substr(char const *s, unsigned int start, int len, int f);

//ft_split.c
char	**ft_split(char const *s, char c);

#endif
