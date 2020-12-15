/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 12:50:21 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/15 18:49:47 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	*life_filosofs(void *vars)
{
	t_vars	*ptr;
	int		i;

	ptr = (t_vars *)vars;
	i = ptr->fil[ptr->count].index;
	while (ptr->flag)
		life_phil_res(ptr, i);
	return (NULL);
}

void	born_phil(t_vars *vars)
{
	while (vars->count < vars->number_of_philosophers)
	{
		pthread_create(&vars->mas_fil[vars->count],
		NULL, life_filosofs, (void *)vars);
		usleep(50);
		if (vars->count == vars->number_of_philosophers)
			break ;
		vars->count++;
	}
}

int		check_fil_res(t_vars *ptr, int *i, int *j, int *count)
{
	gettimeofday(&ptr->check_time, NULL);
	ptr->time_check = ptr->check_time.tv_sec
	* 1000 + ptr->check_time.tv_usec / 1000;
	if (ptr->simulation_counter != 0
	&& ptr->fil[*j].count_cycle == ptr->simulation_counter)
	{
		(*j)++;
		(*count)++;
	}
	if ((ptr->time_check - ptr->fil[*i].tyme_last_eat) > ptr->time_to_die)
	{
		if ((ptr->time_check - ptr->fil[*i].tyme_last_eat) > ptr->time_to_die)
		{
			ptr->flag = 0;
			print(" died\n", *i + 1, ptr, 1);
			return (1);
		}
	}
	if (ptr->simulation_counter != 0 && *count == ptr->number_of_philosophers)
	{
		ptr->flag = 0;
		sem_wait(ptr->print_sem);
		return (1);
	}
	return (0);
}

void	*chek_fil(void *vars)
{
	t_vars	*ptr;
	int		i;
	int		j;
	int		count;

	i = 0;
	j = 0;
	count = 0;
	ptr = (t_vars *)vars;
	while (1)
	{
		while (i < ptr->number_of_philosophers)
		{
			if (check_fil_res(ptr, &i, &j, &count) == 1)
				return (NULL);
			i++;
		}
		i = 0;
		usleep(100);
	}
}

int		main(int argc, char **argv)
{
	t_vars	vars;

	if (argc < 5 || argc > 6)
	{
		write(2, "error: bad arguments\n", ft_strlen("error: bad arguments\n"));
		return (0);
	}
	if (init_vars(argc, argv, &vars) == 1)
	{
		write(2, "error: bad arguments\n", ft_strlen("error: bad arguments\n"));
		return (0);
	}
	pthread_create(&vars.check, NULL, chek_fil, (void *)&vars);
	born_phil(&vars);
	pthread_join(vars.check, NULL);
}
