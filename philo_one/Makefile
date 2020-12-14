# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmiso <hmiso@student.21-school.ru>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/14 21:35:31 by hmiso             #+#    #+#              #
#    Updated: 2020/12/14 21:56:02 by hmiso            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=philo_one

OBJ_FILES =  philo_one.o ft_atoi.o ft_putnbr_fd.o ft_strlen.o ft_isdigit.o ft_putchar_fd.o

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ_FILES) philo_one.h
	gcc $(OBJ_FILES) -o $(NAME)

%.o: %.c 
	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all
