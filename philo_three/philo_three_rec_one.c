/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three_rec_one.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 20:38:25 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/15 20:45:12 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	life_filosofs_res(t_vars *ptr, int i)
{
	if (sem_wait(ptr->sem) != -1)
	{
		if (sem_wait(ptr->sem) == -1)
			sem_post(ptr->sem);
	}
	print(" has taken a fork\n", i, ptr, 0);
	ptr->count_cycle++;
	gettimeofday(&ptr->fil[i - 1].new, NULL);
	print(" is eating\n", i, ptr, 0);
	if (ptr->simulation_counter != 0
	&& ptr->simulation_counter == ptr->count_cycle)
	{
		sem_post(ptr->sem);
		sem_post(ptr->sem);
		exit(1);
	}
	ptr->fil[i - 1].tyme_last_eat = ptr->fil[i - 1].new.tv_sec
	* 1000 + ptr->fil[i - 1].new.tv_usec / 1000;
	usleep(ptr->time_to_eat * 1000);
	sem_post(ptr->sem);
	sem_post(ptr->sem);
	print(" is sleeping\n", i, ptr, 0);
	usleep(ptr->time_to_sleep * 1000);
	print(" is thinking\n", i, ptr, 0);
}

void	*life_filosofs(void *vars)
{
	t_vars	*ptr;
	int		i;

	ptr = (t_vars *)vars;
	i = ptr->fil[ptr->count].index;
	pthread_create(&ptr->check, NULL, chek_fil, (void *)ptr);
	while (1)
		life_filosofs_res(ptr, i);
}

void	born_phil_res(t_vars *vars)
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

void	born_phil(t_vars *vars)
{
	int		i;
	int		status;

	i = 0;
	status = 0;
	while (vars->count < vars->number_of_philosophers)
		born_phil_res(vars);
	while (vars->count > 0)
	{
		waitpid(-1, &status, 0);
		if (status == 0)
		{
			while (i < vars->count)
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
