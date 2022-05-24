/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 18:16:15 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/24 18:17:36 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	get_time(unsigned int *time, unsigned int *b_time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return ;
	if (b_time == NULL)
		*time = tv.tv_sec * 1000000 + tv.tv_usec;
	else
		*time = (tv.tv_sec * 1000000 + tv.tv_usec) - *b_time;
}

int	philoloner(t_data *data)
{
	unsigned int	time;

	get_time(&time, &(data->b_time));
	printf("%u 1 is thinking\n", time / 1000);
	pthread_mutex_lock(data->printf);
	printf("%u 1 has taken a fork\n", time / 1000);
	pthread_mutex_unlock(data->printf);
	while (time < data->t_t_d)
	{
		usleep(500);
		get_time(&time, &(data->b_time));
	}
	printf("%u 1 died\n", time / 1000);
	return (0);
}

int	philosophers(t_data *data)
{
	unsigned int	i;

	get_time(&(data->b_time), NULL);
	if (data->nb_p == 1)
		return (philoloner(data));
	if (ft_init(data))
		return (-1);
	i = 0;
	check_alive(data, i);
	{
		while (i < data->nb_p)
		{
			if (pthread_join(data->philo[i].thread, NULL))
				return (-1);
			i++;
		}
	}
	return (0);
}
