/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_alive.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:07:26 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/24 17:33:34 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	kill_em_all(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nb_p)
	{
		pthread_mutex_lock(&(data->philo[i].is_alive_m));
		data->philo[i].is_alive = 0;
		pthread_mutex_unlock(&(data->philo[i].is_alive_m));
		i++;
	}
}

void	check_fellow(t_philo *philo, unsigned int time_i, unsigned int *ret)
{
	unsigned int	time_p;

	pthread_mutex_lock(&(philo->time_m));
	time_p = philo->p_time;
	pthread_mutex_unlock(&(philo->time_m));
	if (time_p < time_i)
		*ret = 1;
	else
		*ret = 0;
}

void	check_order(t_data *data, unsigned int i)
{
	unsigned int	ret;
	unsigned int	time_i;

	check_fork(data, i, &ret);
	if (ret != 0)
		return ;
	pthread_mutex_lock(&(data->philo[i].time_m));
	time_i = data->philo[i].p_time;
	pthread_mutex_unlock(&(data->philo[i].time_m));
	if (i == 0)
		check_fellow(&(data->philo[data->nb_p - 1]), time_i, &ret);
	else
		check_fellow(&(data->philo[i - 1]), time_i, &ret);
	if (ret)
		return ;
	if (i == data->nb_p - 1)
		check_fellow(&(data->philo[0]), time_i, &ret);
	else
		check_fellow(&(data->philo[i + 1]), time_i, &ret);
	if (ret)
		return ;
	philo_can_eat(data, i);
}

void	check_alive(t_data *data, unsigned int i)
{
	unsigned char	philo_life;
	unsigned int	check_meal;

	check_meal = 0;
	while (7)
	{	
		pthread_mutex_lock(&(data->philo[i].is_alive_m));
		philo_life = data->philo[i].is_alive;
		pthread_mutex_unlock(&(data->philo[i].is_alive_m));
		if (!philo_life)
		{
			kill_em_all(data);
			usleep(2000);
			ft_print(&(data->philo[i]), "died");
			return ;
		}
		check_order(data, i);
		if (data->n_t_e)
			check_nte(data, i, &check_meal);
		if (check_meal == data->nb_p)
			return (kill_em_all(data));
		i++;
		if (i == data->nb_p)
			i = 0;
	}
}
