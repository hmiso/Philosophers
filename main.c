/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 19:50:23 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/08 17:36:45 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>

typedef struct s_phil{
	long int simulation_start_time;
	int last_eat;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philosopher_must_eat;	
	pthread_mutex_t left;
	pthread_mutex_t riht;
	int index;
}				t_phil;

typedef struct s_vars{

	int number_of_philosophers;
	long int simulation_start_time;
	int count;
	long int simulation_start_time;
	int last_eat;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philosopher_must_eat;	
	pthread_t *mas_fil;
	pthread_mutex_t *mutex;
	t_phil *phil;
}				t_vars;



int		ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}

int	ft_strlen(const char *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

static int	ft_char_int(const char *str, unsigned long m, int c, int i)
{
	int	d;

	d = 0;
	while (ft_isdigit(str[i]))
	{
		m = m * 10 + (str[i] - '0');
		if (str[i] - '0' != 0 || d != 0)
			d++;
		if (d > 19 || m > 9223372036854775807)
		{
			if (c > 0)
				return (-1);
			else
				return (0);
		}
		if (!ft_isdigit(str[i + 1]))
			return ((int)m * c);
		i++;
	}
	return (0);
}

int			ft_atoi(const char *str)
{
	int				i;
	unsigned long	m;
	int				c;

	i = 0;
	m = 0;
	c = 1;
	while (str[i] != '\0')
	{
		if (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i]
		== '\f' || str[i] == '\r' || str[i] == ' ')
			i++;
		else if (str[i] == '-' && ft_isdigit(str[i + 1]))
		{
			c = -1;
			i++;
		}
		else if (str[i] == '+' && ft_isdigit(str[i + 1]))
			i++;
		else if (ft_isdigit(str[i]))
			return (ft_char_int(str, m, c, i));
		else
			return (0);
	}
	return (0);
}

void init_vars(int argc, char **argv, t_vars *vars)
{
	struct timeval time;

	vars->number_of_philosophers = ft_atoi(argv[1]);
	// vars->time_to_die = ft_atoi(argv[2]);
	// vars->time_to_eat = ft_atoi(argv[3]);
	// vars->time_to_sleep = ft_atoi(argv[4]);
	vars->count = 0;
	// if (argc == 6)
	// 	vars->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	// else
	// 	vars->number_of_times_each_philosopher_must_eat = 0;
	vars->mas_fil = malloc(sizeof(pthread_t) * vars->number_of_philosophers);
	vars->mutex = malloc(sizeof(pthread_mutex_t) * vars->number_of_philosophers);
	vars->phil = malloc(sizeof(t_phil *) * vars->number_of_philosophers);
	int i = 0;
	while (i < vars->number_of_philosophers)
	{
		vars->phil[i] = malloc(sizeof(t_phil));
		vars->phil[i]->time_to_die = ft_atoi(argv[2]);
		vars->phil[i]->time_to_eat = ft_atoi(argv[3]);
		vars->phil[i]->time_to_sleep = ft_atoi(argv[4]);
		vars->phil[i]->left = vars->mutex[i];
		vars->phil[i]->index = i;
		if (i == vars->number_of_philosophers - 1)
		{
			vars->phil[i]->riht = vars->mutex[0];
		}
		else
		{
			vars->phil[i]->riht = vars->mutex[i + 1];
		}
		pthread_mutex_init(&vars->phil[i]->left, NULL);
		pthread_mutex_init(&vars->phil[i]->riht, NULL);
		i++;
	}
}


void *life_filosofs(void *vars)
{
	pthread_mutex_lock(((t_vars *)vars)->phil->left);
	return NULL;
}

void born_filosofs(t_vars *vars)
{

	while (vars->count < vars->number_of_philosophers)
	{
		vars->count++;
		pthread_create(&vars->mas_fil[vars->count], NULL, life_filosofs, (void *)vars);
		usleep(50);
	}
	vars->count = 0;
}

int main (int argc, char **argv)
{
	t_vars vars;
	int i = 0;
	if (argc < 5 || argc > 6)
	{
		write(2, "error: bad arguments\n", ft_strlen("error: bad arguments\n"));
		return (0);
	}
	init_vars(argc, argv, &vars);
	born_filosofs(&vars);
	// while (i < vars.number_of_philosophers)
	// {
	// 	pthread_join(vars.mas_fil[i], NULL);
	// 	i++;
	// }
}