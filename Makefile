NAME	=	cub3D

LIBFT	=	libft/libft.a

CC		=	clang

FLAGS	=	-O3 -Wall -Wextra -Werror -g3

SRCS	=	srcs/main.c \
			srcs/ft_split.c \
			srcs/get_next_line.c \
			srcs/get_next_line_utils.c

INCS	=	$(addprefix include/, cub3d.h)

OBJS	=	$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(INCS)
	make -C libft
	make -C mlx
	$(CC) $(FLAGS) -I include -o $(NAME) $(OBJS) libft/libft.a -Lmlx -lmlx -lX11 -lbsd -lm -lXext

%.o: %.c $(INCS)
	$(CC) $(FLAGS) -I include -Imlx -c $< -o $@

clean:
	make clean -C mlx
	make fclean -C libft
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re