/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:15:36 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/15 11:22:59 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		argument_cheking(int argc, char **argv, t_vars *vars)
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
	if (vars->number_of_philosophers <= 0)
		return (1);
	if (vars->time_to_die <= 0)
		return (1);
	if (vars->time_to_eat <= 0)
		return (1);
	if (vars->time_to_sleep <= 0)
		return (1);
	if (vars->number_of_times_each_philosopher_must_eat < 0)
		return (1);
	return (0);	
}

int		init_mutex(t_vars *vars)
{
	if (pthread_mutex_init(&vars->print_mutex, NULL))
		return (1);
	vars->mutex = malloc(sizeof(pthread_mutex_t) * vars->number_of_philosophers);
	while (vars->count < vars->number_of_philosophers)
	{
		if (pthread_mutex_init(&vars->mutex[vars->count], NULL))
			return (1);
		vars->count++;
	}
	vars->count = 0;
	return (0);
}

int		init_streams(t_vars *vars)
{
	vars->mas_fil = malloc(sizeof(pthread_t)
	* vars->number_of_philosophers);
	if (vars->mas_fil == NULL)
		return (1);
	vars->fil = (t_fil *)malloc(sizeof(t_fil)
	* vars->number_of_philosophers);
	if (vars->fil == NULL)
		return (1);
	while (vars->count < vars->number_of_philosophers)
	{
		vars->fil[vars->count].index = vars->count + 1;
		vars->fil[vars->count].count_cycle = 0;
		vars->count++;
	}
	vars->count = 0;
	return (0);
}

void		save_start_time(t_vars *vars)
{
	gettimeofday(&vars->old, NULL);
	while(vars->count < vars->number_of_philosophers)
	{
		vars->fil[vars->count].tyme_last_eat =
		vars->old.tv_sec * 1000 + vars->old.tv_usec / 1000;
		vars->count++;
	}
	vars->simulation_start_time =
	vars->old.tv_sec * 1000 + vars->old.tv_usec / 1000;
	vars->count = 0;
	return (0);
}

int		init_vars(int argc, char **argv, t_vars *vars)
{
	if (argument_cheking(argc, argv, vars) == 1)
		return (1);
	if (init_mutex(vars) == 1)
	{
		write(2, "Initialization error mutex\n",
		ft_strlen("Initialization error mutex\n"));
		return (1);
	}
	if (init_streams(vars) == 1)
	{
		write(2, "Initialization error streams\n",
		ft_strlen("Initialization error streams\n"));
		return (1);
	}
	save_start_time(vars);
}

void print(char *str, int i, t_vars *vars)
{
	struct		timeval print_time;

	pthread_mutex_lock(&vars->print_mutex);
	gettimeofday(&print_time, NULL);
	ft_putnbr_fd((print_time.tv_sec * 1000 + print_time.tv_usec
	/ 1000 - vars->simulation_start_time), 1);
	write(1, " ", 1);
	ft_putnbr_fd(i, 1);
	write(1, str, ft_strlen(str));
	pthread_mutex_unlock(&vars->print_mutex);
}

void	life_phil_res(t_vars *ptr, int l, int r, int i)
{
		if(!(pthread_mutex_lock(&ptr->mutex[l])))
			pthread_mutex_lock(&ptr->mutex[r]);
		print(" has taken a fork\n", i, ptr);
		ptr->fil[i - 1].count_cycle++;
		gettimeofday(&ptr->fil[i - 1].new, NULL);
		print(" is eating\n", i, ptr);
		ptr->fil[i - 1].tyme_last_eat = ptr->fil[i - 1].new.tv_sec * 1000 + ptr->fil[i - 1].new.tv_usec / 1000;
		usleep(ptr->time_to_eat * 1000);
		pthread_mutex_unlock(&ptr->mutex[l]);
		pthread_mutex_unlock(&ptr->mutex[r]);
		print(" is sleeping\n", i, ptr);
		usleep(ptr->time_to_sleep * 1000);
		print(" is thinking\n", i, ptr);
}

void *life_filosofs(void *vars)
{
	t_vars *ptr;
	int i;
	int l;
	int r;

	ptr = (t_vars *)vars;
	i = ptr->fil[ptr->count].index;
	l = i - 2;
	r = i - 1;
	if (i == 1)
	{
		r = 0;
		l = ((t_vars *)vars)->number_of_philosophers - 1;
	}
	while (1)
	{

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
	int j = 0;
	int count = 0;
	ptr = (t_vars *)vars;
	while(1)
	{
		while(i < ptr->number_of_philosophers)
		{
			gettimeofday(&ptr->check_time, NULL);
			ptr->time_check = ptr->check_time.tv_sec * 1000 + ptr->check_time.tv_usec / 1000;
			if (ptr->number_of_times_each_philosopher_must_eat != 0 && ptr->fil[j].count_cycle == ptr->number_of_times_each_philosopher_must_eat)
			{
				j++;
				count++;
			}
			if ((ptr->time_check - ptr->fil[i].tyme_last_eat) > ptr->time_to_die)
			{
				usleep(100);
				if ((ptr->time_check - ptr->fil[i].tyme_last_eat) > ptr->time_to_die)
				{
					print(" died\n", i + 1, ptr);
					return NULL;
				}
			}
			if (ptr->number_of_times_each_philosopher_must_eat != 0 && count == ptr->number_of_philosophers)
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

	if (argc < 5 || argc > 6)
	{
		write(2, "error: bad arguments\n", ft_strlen("error: bad arguments\n"));
		return (0);
	}
	if (init_vars(argc, argv, &vars) == 1)
		write(2, "error: bad arguments\n", ft_strlen("error: bad arguments\n"));
	pthread_create(&vars.check, NULL, chek_fil, (void *)&vars);
	born_phil(&vars);
	pthread_join(vars.check, NULL);
}
