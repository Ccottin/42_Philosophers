NAME	= Philosophers

SRC	= ./main.c

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
