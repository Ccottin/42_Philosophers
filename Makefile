NAME	= Philosophers

SRC	= src/main.c	src/Philosophers.c	src/ft_init.c\
	src/ft_return.c	src/utils.c

HEADER	= ./philosophers.h

OBJ	= $(SRC:%.c=%.o)
		
CC	= gcc

CFLAG	= -Wall -Wextra -Werror -g3

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
