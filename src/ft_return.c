/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_return.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 14:45:24 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/20 13:30:48 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../philosophers.h"

int	free_all(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nb_p)
	{
		if (pthread_mutex_destroy(&(data->philo[i].fork_m)))
			return (-1);
	//	if (pthread_mutex_destroy(&(data->philo[i].politely_wait_m)))
	//		return (-1);
		if (pthread_mutex_destroy(&(data->philo[i].time_m)))
			return (-1);
		if (pthread_mutex_destroy(&(data->philo[i].nb_eat_m)))
			return (-1);
		i++;
	}
	if (pthread_mutex_destroy(data->printf))
		return (-1);
	if (pthread_mutex_destroy(data->l_data))
		return (-1);
	if (pthread_mutex_destroy(data->is_alive_m))
		return (-1);
	if (pthread_mutex_destroy(data->dead_m))
		return (-1);
	free(data->printf);
	free(data->is_alive_m);
	free(data->l_data);
	free(data->dead_m);
	free(data->philo);
	return (0);
}

char	ft_return(char mark, t_data *data)
{
	if (mark == 0)
	{
		printf("Arg requireds : [number_of_philosophers] [time_to_die] [time_to_eat]");
		printf("[time_to_sleep] (opt.)[number_of_time_each_philosopher_must_eat]\n");
		return (0);
	}
	if (mark == 1)
	{
		if (free_all(data))
			return (-1);
	}
	if (mark == -1)
	{
		write(1, "An error has occured.\n", 22);
		free_all(data);
	}
	return (0);
}
