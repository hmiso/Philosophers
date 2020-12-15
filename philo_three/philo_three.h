/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 21:33:12 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/15 21:00:04 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdio.h>
# include <semaphore.h>
# include <signal.h>

typedef struct		s_fil{
	int				index;
	long int		tyme_last_eat;
	int				count_cycle;
	struct timeval	new;
}					t_fil;

typedef struct		s_vars{

	int				number_of_philosophers;
	long int		simulation_start_time;
	int				count;
	int				last_eat;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				simulation_counter;
	long int		time_start;
	pid_t			*pid;
	t_fil			*fil;
	struct timeval	old;
	struct timeval	check_time;
	pthread_t		check;
	long int		time_check;
	sem_t			*print_sem;
	sem_t			*sem;
	int				count_cycle;
}					t_vars;

void				ft_putnbr_fd(int n, int fd);
int					ft_atoi(const char *str);
size_t				ft_strlen(const char *s);
int					ft_isdigit(int c);
void				ft_putchar_fd(char c, int fd);
int					argument_cheking(int argc, char **argv, t_vars *vars);
int					init_streams(t_vars *vars);
void				save_start_time(t_vars *vars);
int					init_vars(int argc, char **argv, t_vars *vars);
void				print(char *str, int i, t_vars *vars, int flag);
void				life_filosofs_res(t_vars *ptr, int i);
void				*life_filosofs(void *vars);
void				born_phil_res(t_vars *vars);
void				born_phil(t_vars *vars);
void				*chek_fil(void *vars);

#endif
