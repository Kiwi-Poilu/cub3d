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

	$(CC) $(FLAGS) -I include -o $(NAME) $(OBJS) libft/libft.a
%.o: %.c $(INCS)
	$(CC) $(FLAGS) -I include -c $< -o $@

clean:
	make fclean -C libft
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re