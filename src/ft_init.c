/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 20:17:20 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/24 17:29:32 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	fillo(t_philo *philo, t_data *data, unsigned int nb)
{
	philo->nb = nb + 1;
	philo->t_t_d = data->t_t_d;
	philo->t_t_s = data->t_t_s;
	if (data->t_t_d < data->t_t_e)
		philo->t_t_e = data->t_t_d;
	else
		philo->t_t_e = data->t_t_e;
	philo->n_t_e = data->n_t_e;
	philo->nb_eat = 0;
	philo->nb_p = data->nb_p;
	philo->printf = data->printf;
	philo->is_alive = 1;
	philo->fork = 0;
	philo->b_time = data->b_time;
	get_time(&(philo->p_time), &(data->b_time));
	if (data->nb_p != 1 && nb == data->nb_p - 1)
	{
		philo->fork1 = &(data->philo[0].fork);
		philo->fork1_m = &(data->philo[0].fork_m);
	}
	else
	{
		philo->fork1 = &(data->philo[nb + 1].fork);
		philo->fork1_m = &(data->philo[nb + 1].fork_m);
	}
}

int	init_philo(unsigned int nb, t_data *data)
{
	t_philo	philo;

	fillo(&philo, data, nb);
	pthread_mutex_lock(data->l_data);
	data->philo[nb] = philo;
	if (pthread_mutex_init(&(data->philo[nb].fork_m), NULL))
		return (-1);
	if (pthread_mutex_init(&(data->philo[nb].nb_eat_m), NULL))
		return (-1);
	if (pthread_mutex_init(&(data->philo[nb].is_alive_m), NULL))
		return (-1);
	if (pthread_mutex_init(&(data->philo[nb].time_m), NULL))
		return (-1);
	pthread_mutex_unlock(data->l_data);
	return (0);
}

void	set_fork(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nb_p)
	{
		if (data->philo[i].nb % 2 == 0)
		{
			pthread_mutex_lock(&(data->philo[i].fork_m));
			data->philo[i].fork = data->philo[i].nb;
			pthread_mutex_unlock(&(data->philo[i].fork_m));
			pthread_mutex_lock(data->philo[i].fork1_m);
			*(data->philo[i].fork1) = data->philo[i].nb;
			pthread_mutex_unlock(data->philo[i].fork1_m);
		}
		i++;
	}
}

int	set_calloc(t_data *data)
{
	t_philo	*philo;

	philo = ft_calloc(sizeof(t_philo) * data->nb_p + 1);
	if (!philo)
		return (-1);
	data->philo = philo;
	data->l_data = ft_calloc(sizeof(pthread_mutex_t));
	data->printf = ft_calloc(sizeof(pthread_mutex_t));
	if (!data->printf || !data->l_data)
		return (-1);
	if (pthread_mutex_init(data->l_data, NULL))
		return (-1);
	if (pthread_mutex_init(data->printf, NULL))
		return (-1);
	return (0);
}

int	ft_init(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nb_p)
	{
		init_philo(i, data);
		i++;
	}
	set_fork(data);
	i = 0;
	while (i < data->nb_p)
	{
		if (pthread_create(&(data->philo[i].thread), NULL, &alive,
				(void *)&(data->philo[i])))
			return (-1);
		i++;
	}
	return (0);
}
