NAME	= philo

SRC	= src/main.c	src/philosophers.c	src/ft_init.c		src/alive.c	src/eat.c\
	src/ft_return.c	src/utils.c		src/check_alive.c	src/check_alive_utils.c\

HEADER	= ./philosophers.h

OBJ	= $(SRC:%.c=%.o)
		
CC	= gcc

CFLAG	= -Wall -Wextra -Werror  #-g3 -fsanitize=address

all	:	$(NAME)

%.o	:	%.c
		$(CC) $(CFLAG) -I./ -c $^ -o $@

$(NAME)	:	$(OBJ)
		$(CC) $(CFLAG) $(OBJ) -lpthread -o $(NAME)

clean	:
		rm -rf $(OBJ)

fclean	:	clean
		rm -rf $(NAME)

re	:	fclean all

.PHONY	:	all	clean	fclean	re
