# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/14 21:35:31 by hmiso             #+#    #+#              #
#    Updated: 2020/12/15 18:40:46 by hmiso            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=philo_two

OBJ_FILES =  philo_two.o ft_atoi.o ft_putnbr_fd.o ft_strlen.o ft_isdigit.o ft_putchar_fd.o \
			 philo_two_rec.o philo_two_rec_one.o 

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ_FILES) philo_two.h
	gcc $(OBJ_FILES) -o $(NAME)

%.o: %.c 
	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all
