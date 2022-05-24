/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_alive_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:19:12 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/24 17:30:59 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	philo_can_eat(t_data *data, unsigned int i)
{	
	pthread_mutex_lock(&(data->philo[i].fork_m));
	data->philo[i].fork = data->philo[i].nb;
	pthread_mutex_unlock(&(data->philo[i].fork_m));
	pthread_mutex_lock(data->philo[i].fork1_m);
	*(data->philo[i].fork1) = data->philo[i].nb;
	pthread_mutex_unlock(data->philo[i].fork1_m);
}

void	check_fork(t_data *data, unsigned int i, unsigned int *ret)
{
	pthread_mutex_lock(&(data->philo[i].fork_m));
	*ret = data->philo[i].fork;
	pthread_mutex_unlock(&(data->philo[i].fork_m));
	if (*ret != 0)
		return ;
	pthread_mutex_lock(data->philo[i].fork1_m);
	*ret = *(data->philo[i].fork1);
	pthread_mutex_unlock(data->philo[i].fork1_m);
}

void	check_nte(t_data *data, unsigned int i, unsigned int *check_meal)
{
	unsigned int	temp;

	pthread_mutex_lock(&(data->philo[i].nb_eat_m));
	temp = data->philo[i].nb_eat;
	pthread_mutex_unlock(&(data->philo[i].nb_eat_m));
	if (temp >= data->n_t_e)
		*check_meal = *check_meal + 1;
	else
		*check_meal = 0;
}
