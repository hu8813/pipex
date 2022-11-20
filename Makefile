# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: huaydin <huaydin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/17 13:21:50 by huaydin           #+#    #+#              #
#    Updated: 2022/11/20 06:42:03 by huaydin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= cc
NAME	= pipex
SRC		= pipex.c
FLAGS	= -c $< -o $@
CFLAGS	= -Wall -Wextra -Werror -g
OBJ		= $(SRC:.c=.o)

.PHONY:	all clean fclean re

%.o: %.c
	$(CC) $(CFLAGS) $(FLAGS)
$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)
fclean:clean
	rm -f $(NAME)
re:fclean $(NAME)
