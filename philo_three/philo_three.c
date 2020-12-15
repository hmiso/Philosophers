/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:19:07 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/15 20:42:25 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	*chek_fil(void *vars)
{
	t_vars	*ptr;

	ptr = (t_vars *)vars;
	while (1)
	{
		gettimeofday(&ptr->check_time, NULL);
		ptr->time_check = ptr->check_time.tv_sec * 1000
		+ ptr->check_time.tv_usec / 1000;
		if ((ptr->time_check - ptr->fil[ptr->count].tyme_last_eat)
		> ptr->time_to_die)
		{
			if ((ptr->time_check - ptr->fil[ptr->count].tyme_last_eat)
			> ptr->time_to_die)
			{
				print(" died\n", ptr->count + 1, ptr, 1);
				exit(0);
			}
		}
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
	init_vars(argc, argv, &vars);
	born_phil(&vars);
	pthread_join(vars.check, NULL);
}
