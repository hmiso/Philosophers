/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three_rec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 20:34:39 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/15 20:34:43 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		argument_cheking(int argc, char **argv, t_vars *vars)
{
	vars->number_of_philosophers = ft_atoi(argv[1]);
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	vars->count = 0;
	if (argc == 6)
		vars->simulation_counter = ft_atoi(argv[5]);
	else
		vars->simulation_counter = 0;
	if (vars->number_of_philosophers <= 0)
		return (1);
	if (vars->time_to_die <= 0)
		return (1);
	if (vars->time_to_eat <= 0)
		return (1);
	if (vars->time_to_sleep <= 0)
		return (1);
	if (vars->simulation_counter < 0)
		return (1);
	return (0);
}

int		init_streams(t_vars *vars)
{
	vars->count = 0;
	vars->fil = (t_fil *)malloc(sizeof(t_fil) * vars->number_of_philosophers);
	if (vars->fil == NULL)
		return (1);
	while (vars->count < vars->number_of_philosophers)
	{
		vars->fil[vars->count].index = vars->count + 1;
		vars->fil[vars->count].count_cycle = 0;
		vars->count++;
	}
	return (0);
}

void	save_start_time(t_vars *vars)
{
	gettimeofday(&vars->old, NULL);
	while (vars->count < vars->number_of_philosophers)
	{
		vars->fil[vars->count].tyme_last_eat =
		vars->old.tv_sec * 1000 + vars->old.tv_usec / 1000;
		vars->count++;
	}
	vars->simulation_start_time =
	vars->old.tv_sec * 1000 + vars->old.tv_usec / 1000;
	vars->count = 0;
}

int		init_vars(int argc, char **argv, t_vars *vars)
{
	if (argument_cheking(argc, argv, vars) == 1)
		return (1);
	vars->pid = malloc(sizeof(pid_t) * vars->number_of_philosophers);
	if (init_streams(vars) == 1)
	{
		write(2, "Initialization error malloc\n",
		ft_strlen("Initialization error malloc\n"));
		return (1);
	}
	sem_unlink("print");
	vars->print_sem = sem_open("print", O_CREAT, 0666, 1);
	sem_unlink("sem");
	vars->sem = sem_open("sem", O_CREAT, 0666, vars->number_of_philosophers);
	vars->count = 0;
	save_start_time(vars);
	return (0);
}

void	print(char *str, int i, t_vars *vars, int flag)
{
	struct timeval	print_time;

	sem_wait(vars->print_sem);
	gettimeofday(&print_time, NULL);
	ft_putnbr_fd((print_time.tv_sec * 1000 + print_time.tv_usec / 1000
	- vars->simulation_start_time), 1);
	write(1, " ", 1);
	ft_putnbr_fd(i, 1);
	write(1, str, ft_strlen(str));
	if (flag != 1)
		sem_post(vars->print_sem);
}
