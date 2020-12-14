/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 21:33:12 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/14 21:57:53 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>

typedef struct	s_fil{
	int index;
	long int tyme_last_eat;
	int			count_cycle;
	struct		timeval new;
}				t_fil;

typedef struct s_vars{

	int number_of_philosophers;
	long int simulation_start_time;
	int count;
	int last_eat;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
	long int time_start;
	pthread_t *mas_fil;
	pthread_mutex_t *mutex;
	t_fil *fil;
	struct		timeval old;
	struct		timeval check_time;
	pthread_t check;
	long int time_check;
	pthread_mutex_t print_mutex;
}				t_vars;

void	ft_putnbr_fd(int n, int fd);
int			ft_atoi(const char *str);
size_t	ft_strlen(const char *s);
int		ft_isdigit(int c);
void	ft_putchar_fd(char c, int fd);