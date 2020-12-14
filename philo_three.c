/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 18:56:09 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/14 17:22:59 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <signal.h>
#include "libft/libft.h"


void *chek_fil(void *vars);

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
	pid_t *pid;
	t_fil *fil;
	struct		timeval old;
	struct		timeval check_time;
	pthread_t check;
	long int time_check;
	sem_t *print_sem;
	sem_t *sem;
	sem_t *block_print_die;
	int count_cycle;
}				t_vars;

void init_vars(int argc, char **argv, t_vars *vars)
{

	vars->number_of_philosophers = ft_atoi(argv[1]);
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	vars->count = 0;
	vars->count_cycle = 0;
	if (argc == 6)
		vars->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		vars->number_of_times_each_philosopher_must_eat = 0;
	vars->mas_fil = malloc(sizeof(pthread_t) * vars->number_of_philosophers);
	vars->pid = malloc(sizeof(pid_t) * vars->number_of_philosophers);
	vars->count = 0;
	vars->fil = (t_fil *)malloc(sizeof(t_fil) * vars->number_of_philosophers);
	while(vars->count < vars->number_of_philosophers)
	{
		vars->fil[vars->count].index = vars->count + 1;
		vars->fil[vars->count].count_cycle = 0;
		vars->count++;
	}
	sem_unlink("print");
	vars->print_sem = sem_open("print", O_CREAT, 0666, 1);
	sem_unlink("sem");
	vars->sem = sem_open("sem", O_CREAT, 0666, vars->number_of_philosophers);
	vars->count = 0;
	gettimeofday(&vars->old, NULL);
	while(vars->count < vars->number_of_philosophers)
	{
		vars->fil[vars->count].tyme_last_eat = vars->old.tv_sec * 1000 + vars->old.tv_usec / 1000;
		vars->count++;
	}
	vars->simulation_start_time = vars->old.tv_sec * 1000 + vars->old.tv_usec / 1000;
	vars->count = 0;
	sem_unlink("block_print_die");
	vars->block_print_die = sem_open("block_print_die", O_CREAT, 0666, 1);	
}

void print(char *str, int i, t_vars *vars, int flag)
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
	if (flag != 1)
		sem_post(vars->print_sem);
}

void *life_filosofs(void *vars)
{
	t_vars *ptr;
	pthread_t check;
	ptr = (t_vars *)vars;
	int i = ptr->fil[ptr->count].index;
	int l = i - 2;
	int r = i - 1;
	pthread_create(&ptr->check, NULL, chek_fil, (void *)ptr);
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
		print(" has taken a fork\n", i, ptr, 0);
		ptr->count_cycle++;
		gettimeofday(&ptr->fil[i - 1].new, NULL);
		print(" is eating\n", i, ptr, 0);
		if (ptr->number_of_times_each_philosopher_must_eat != 0 && ptr->number_of_times_each_philosopher_must_eat == ptr->count_cycle)
		{
			sem_post(ptr->sem);
			sem_post(ptr->sem);
			exit(1);
		}
		ptr->fil[i - 1].tyme_last_eat = ptr->fil[i - 1].new.tv_sec * 1000 + ptr->fil[i - 1].new.tv_usec / 1000;
		usleep(((t_vars*)vars)->time_to_eat * 1000);
		sem_post(ptr->sem);
		sem_post(ptr->sem);
		print(" is sleeping\n", i, ptr, 0);
		usleep(((t_vars*)vars)->time_to_sleep * 1000);
		print(" is thinking\n", i, ptr, 0);
	}
	exit (1);
}

void born_phil(t_vars *vars)
{
	int i = 0;
	int status = 0;
	while (vars->count < vars->number_of_philosophers)
	{
		vars->pid[vars->count] = fork();
		if (vars->pid[vars->count] < 0)
		{
			print("Error", 0, vars, 0);
			exit(0);
		}
		if (vars->pid[vars->count] == 0)
			life_filosofs(vars);
		vars->count++;
	}
	while(vars->count > 0)
	{
		waitpid(-1, &status, 0);
		if (status == 0)
		{
			while(i < vars->count)
			{
				kill(vars->pid[i], SIGINT);
				i++;
			}
			exit(0);
		}
		else
			vars->count--;
	}
}

void *chek_fil(void *vars)
{
	t_vars *ptr;
	int i = 0;
	ptr = (t_vars *)vars;
	while(1)
	{
		gettimeofday(&ptr->check_time, NULL);
		ptr->time_check = ptr->check_time.tv_sec * 1000 + ptr->check_time.tv_usec / 1000;
		if ((ptr->time_check - ptr->fil[ptr->count].tyme_last_eat) > ptr->time_to_die)
		{
			// usleep(100);
			if ((ptr->time_check - ptr->fil[ptr->count].tyme_last_eat) > ptr->time_to_die)
			{
				print(" died\n", ptr->count + 1, ptr, 1);
				exit(0);
			}
		}
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
	born_phil(&vars);
	pthread_join(vars.check, NULL);
}
