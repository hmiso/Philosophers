/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two_rec_one.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 18:40:03 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/15 18:44:04 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	life_phil_res(t_vars *ptr, int i)
{
	if (sem_wait(ptr->sem) != -1)
		if (sem_wait(ptr->sem) == -1)
			sem_post(ptr->sem);
	if (ptr->flag)
		print(" has taken a fork\n", i, ptr, 0);
	ptr->fil[i - 1].count_cycle++;
	gettimeofday(&ptr->fil[i - 1].new, NULL);
	if (ptr->flag)
		print(" is eating\n", i, ptr, 0);
	ptr->fil[i - 1].tyme_last_eat = ptr->fil[i - 1].new.tv_sec
	* 1000 + ptr->fil[i - 1].new.tv_usec / 1000;
	usleep(ptr->time_to_eat * 1000);
	sem_post(ptr->sem);
	sem_post(ptr->sem);
	if (ptr->flag)
		print(" is sleeping\n", i, ptr, 0);
	usleep(ptr->time_to_sleep * 1000);
	if (ptr->flag)
		print(" is thinking\n", i, ptr, 0);
}
