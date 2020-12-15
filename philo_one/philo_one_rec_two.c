/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one_rec_two.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmiso <hmiso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 13:41:36 by hmiso             #+#    #+#             */
/*   Updated: 2020/12/15 13:41:59 by hmiso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	print(char *str, int i, t_vars *vars, int flag)
{
	struct timeval	print_time;

	pthread_mutex_lock(&vars->print_mutex);
	gettimeofday(&print_time, NULL);
	ft_putnbr_fd((print_time.tv_sec * 1000 + print_time.tv_usec
	/ 1000 - vars->simulation_start_time), 1);
	write(1, " ", 1);
	ft_putnbr_fd(i, 1);
	write(1, str, ft_strlen(str));
	if (flag == 0)
		pthread_mutex_unlock(&vars->print_mutex);
}

void	life_phil_res(t_vars *ptr, int l, int r, int i)
{
	if (!(pthread_mutex_lock(&ptr->mutex[l])))
		pthread_mutex_lock(&ptr->mutex[r]);
	if (ptr->flag)
		print(" has taken a fork\n", i, ptr, 0);
	ptr->fil[i - 1].count_cycle++;
	gettimeofday(&ptr->fil[i - 1].new, NULL);
	if (ptr->flag)
		print(" is eating\n", i, ptr, 0);
	ptr->fil[i - 1].tyme_last_eat = ptr->fil[i - 1].new.tv_sec
	* 1000 + ptr->fil[i - 1].new.tv_usec / 1000;
	usleep(ptr->time_to_eat * 1000);
	pthread_mutex_unlock(&ptr->mutex[l]);
	pthread_mutex_unlock(&ptr->mutex[r]);
	if (ptr->flag)
		print(" is sleeping\n", i, ptr, 0);
	usleep(ptr->time_to_sleep * 1000);
	if (ptr->flag)
		print(" is thinking\n", i, ptr, 0);
}
