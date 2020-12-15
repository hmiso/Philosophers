# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/14 21:35:31 by hmiso             #+#    #+#              #
#    Updated: 2020/12/15 20:42:03 by hmiso            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=philo_three

OBJ_FILES =  philo_three.o ft_atoi.o ft_putnbr_fd.o ft_strlen.o ft_isdigit.o ft_putchar_fd.o \
			 philo_three_rec.o philo_three_rec_one.o

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ_FILES) philo_three.h
	gcc $(OBJ_FILES) -o $(NAME)

%.o: %.c 
	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all
