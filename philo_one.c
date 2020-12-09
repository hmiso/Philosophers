/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:15:36 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/09 22:43:36 by hmiso            ###   ########.fr       */
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
	struct    timeval new;
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
	struct    timeval old;
	struct    timeval check_time;
	pthread_t check;
	long int time_check;
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

	vars->number_of_philosophers = ft_atoi(argv[1]);
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	vars->count = 0;
	if (argc == 6)
		vars->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		vars->number_of_times_each_philosopher_must_eat = 0;
	vars->mas_fil = malloc(sizeof(pthread_t) * vars->number_of_philosophers);
	vars->mutex = malloc(sizeof(pthread_mutex_t) * vars->number_of_philosophers);
	while(vars->count < vars->number_of_philosophers)
	{
		pthread_mutex_init(&vars->mutex[vars->count], NULL);
		vars->count++;
	}
	vars->count = 0;
	vars->fil = (t_fil *)malloc(sizeof(t_fil *) * vars->number_of_philosophers);
	while(vars->count < vars->number_of_philosophers)
	{
		vars->fil[vars->count].index = vars->count + 1;
		vars->count++;
	}
	vars->count = 0;
	gettimeofday(&vars->old, NULL);
	while(vars->count < vars->number_of_philosophers)
	{
		vars->fil[vars->count].tyme_last_eat = vars->old.tv_sec * 1000 + vars->old.tv_usec / 1000;
		vars->count++;
	}
	vars->simulation_start_time = vars->old.tv_sec * 1000 + vars->old.tv_usec / 1000;
	vars->count = 0;
}

void *life_filosofs(void *vars)
{
	t_vars *ptr;

	ptr = (t_vars *)vars;
	int i = ptr->fil[ptr->count].index;
	int l = i - 2;
	int r = i - 1;
	if (i == 1)
	{
		r = 0;
		l = ((t_vars *)vars)->number_of_philosophers - 1;
	}
	while (1)
	{
		pthread_mutex_lock(&((t_vars *)vars)->mutex[l]);
		pthread_mutex_lock(&((t_vars *)vars)->mutex[r]);
		printf("i am phil - %d i take fork L- %d R- %d\n", i, l, r);
		gettimeofday(&ptr->fil[i - 1].new, NULL);
		ptr->fil[i - 1].tyme_last_eat = ptr->fil[i - 1].new.tv_sec * 1000 + ptr->fil[i - 1].new.tv_usec / 1000;
		printf("%ld - i eat %d\n", ptr->fil[i - 1].tyme_last_eat - ptr->simulation_start_time, i);
		usleep(((t_vars*)vars)->time_to_eat * 1000);
		pthread_mutex_unlock(&((t_vars *)vars)->mutex[l]);
		pthread_mutex_unlock(&((t_vars *)vars)->mutex[r]);
		printf("i sleep %d\n", i);
		usleep(((t_vars*)vars)->time_to_sleep * 1000);
	}
	return NULL;
}

void born_phil(t_vars *vars)
{
	while (vars->count < vars->number_of_philosophers)
	{
		pthread_create(&vars->mas_fil[vars->count], NULL, life_filosofs, (void *)vars);
		usleep(50);
		if (vars->count == vars->number_of_philosophers)
			break;
		vars->count++;
	}
}

void *chek_fil(void *vars)
{
	t_vars *ptr;
	int i = 0;

	ptr = (t_vars *)vars;
	while(1)
	{
		while(i < ptr->number_of_philosophers)
		{
			gettimeofday(&ptr->check_time, NULL);
			ptr->time_check = ptr->check_time.tv_sec * 1000 + ptr->check_time.tv_usec / 1000;
			if ((ptr->time_check - ptr->fil[i].tyme_last_eat) >= ptr->time_to_die)
			{
				printf ("%ld - i die - %d\n", ptr->time_check - ptr->simulation_start_time, i + 1);
				exit (0);
			}
			i++;
		}
		i = 0;
		usleep(100);
	}
}


int main(int argc, char **argv)
{
	t_vars vars;

	int i = 0;
	if (argc < 5 || argc > 6)
	{
		write(2, "error: bad arguments\n", ft_strlen("error: bad arguments\n"));
		return (0);
	}
	init_vars(argc, argv, &vars);
	pthread_create(&vars.check, NULL, chek_fil, (void *)&vars);
	born_phil(&vars);
	while (i < vars.number_of_philosophers)
	{
		pthread_join(vars.mas_fil[i], NULL);
		i++;
	}
}
