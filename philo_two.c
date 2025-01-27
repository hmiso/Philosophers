/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 12:50:21 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/15 14:19:49 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>


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
	sem_t *print_sem;
	sem_t *sem;
}				t_vars;

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
	vars->count = 0;
	vars->fil = (t_fil *)malloc(sizeof(t_fil) * vars->number_of_philosophers);
	while(vars->count < vars->number_of_philosophers)
	{
		vars->fil[vars->count].index = vars->count + 1;
		vars->fil[vars->count].count_cycle = 0;
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
	sem_unlink("sem");
	vars->sem = sem_open("sem", O_CREAT, 0666, vars->number_of_philosophers);
	sem_unlink("print");
	vars->print_sem = sem_open("print", O_CREAT, 0666, 1);
}

void print(char *str, int i, t_vars *vars)
{
	struct		timeval print_time;
	int			count;
	char		*print_str;
	char		*ptr;
	char		*ptr_free;
	
	sem_wait(vars->print_sem);
	gettimeofday(&print_time, NULL);
	ft_putnbr_fd((print_time.tv_sec * 1000 + print_time.tv_usec / 1000 - vars->simulation_start_time), 1);
	write(1, " ", 1);
	ft_putnbr_fd(i, 1);
	write(1, str, ft_strlen(str));
	sem_post(vars->print_sem);
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
		if (sem_wait(ptr->sem) != -1)
		{
			if(sem_wait(ptr->sem) == -1)
				sem_post(ptr->sem);
		}
		print(" has taken a fork\n", i, ptr);
		ptr->fil[i].count_cycle++;
		gettimeofday(&ptr->fil[i - 1].new, NULL);
		print(" is eating\n", i, ptr);
		ptr->fil[i - 1].tyme_last_eat = ptr->fil[i - 1].new.tv_sec * 1000 + ptr->fil[i - 1].new.tv_usec / 1000;
		usleep(((t_vars*)vars)->time_to_eat * 1000);
		sem_post(ptr->sem);
		sem_post(ptr->sem);
		print(" is sleeping\n", i, ptr);
		usleep(((t_vars*)vars)->time_to_sleep * 1000);
		print(" is thinking\n", i, ptr);
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
	int count;
	ptr = (t_vars *)vars;
	while(1)
	{
		while(i < ptr->number_of_philosophers)
		{
			gettimeofday(&ptr->check_time, NULL);
			ptr->time_check = ptr->check_time.tv_sec * 1000 + ptr->check_time.tv_usec / 1000;
			if(ptr->number_of_times_each_philosopher_must_eat != 0 && ptr->fil[i].count_cycle == ptr->number_of_philosophers)
				count++;
			if ((ptr->time_check - ptr->fil[i].tyme_last_eat) > ptr->time_to_die)
			{
				usleep(100);
				if ((ptr->time_check - ptr->fil[i].tyme_last_eat) > ptr->time_to_die)
				{
					print(" died\n", i + 1, ptr);
					return NULL;
				}
			}
			if (ptr->number_of_times_each_philosopher_must_eat != 0 && count == ptr->number_of_times_each_philosopher_must_eat)
				return NULL;
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
	pthread_join(vars.check, NULL);
}
