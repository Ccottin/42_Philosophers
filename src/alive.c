/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alive.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 17:25:31 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/24 17:28:55 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	ft_print(t_philo *philo, char *str)
{
	unsigned int	time;

	pthread_mutex_lock(&(philo->is_alive_m));
	if (!philo->is_alive && ft_strcmp("died", str))
	{
		pthread_mutex_unlock(&(philo->is_alive_m));
		return ;
	}
	pthread_mutex_unlock(&(philo->is_alive_m));
	pthread_mutex_lock(philo->printf);
	get_time(&time, &(philo->b_time));
	printf("%u %u %s\n", time / 1000, philo->nb, str);
	pthread_mutex_unlock(philo->printf);
}

void	still_breathing(t_philo *philo, char *life)
{
	unsigned int	time;
	unsigned int	philo_time;

	pthread_mutex_lock(&(philo->is_alive_m));
	*life = philo->is_alive;
	pthread_mutex_unlock(&(philo->is_alive_m));
	if (*life == 0)
		return ;
	get_time(&time, &(philo->b_time));
	pthread_mutex_lock(&(philo->time_m));
	philo_time = philo->p_time;
	pthread_mutex_unlock(&(philo->time_m));
	if (philo_time + philo->t_t_d <= time)
	{
		pthread_mutex_lock(&(philo->is_alive_m));
		philo->is_alive = 0;
		pthread_mutex_unlock(&(philo->is_alive_m));
		*life = 0;
	}
}

void	f_sleep(t_philo *philo, char *life)
{
	unsigned int	sleep;
	unsigned int	time;

	ft_print(philo, "is sleeping");
	get_time(&time, &philo->b_time);
	sleep = philo->t_t_s + time;
	still_breathing(philo, life);
	while (*life && time < sleep)
	{
		usleep(750);
		still_breathing(philo, life);
		get_time(&time, &philo->b_time);
	}
}

void	think(t_philo *philo, char *life)
{
	unsigned int	fork;

	pthread_mutex_lock(&(philo->fork_m));
	fork = philo->fork;
	pthread_mutex_unlock(&(philo->fork_m));
	while (*life && fork != philo->nb)
	{
		usleep(750);
		pthread_mutex_lock(&(philo->fork_m));
		fork = philo->fork;
		pthread_mutex_unlock(&(philo->fork_m));
		still_breathing(philo, life);
	}
}

void	*alive(void *ptr)
{
	t_philo	*philo;
	char	life;

	philo = (t_philo *)ptr;
	life = 1;
	if (philo->nb % 2 != 0)
		usleep(philo->t_t_e);
	still_breathing(philo, &life);
	while (life)
	{
		if (life)
			think(philo, &life);
		if (life && philo->nb == philo->nb_p)
			eat_last(philo, &life);
		else if (life && philo->nb != philo->nb_p)
			eat(philo, &life);
		still_breathing(philo, &life);
		if (life)
			f_sleep(philo, &life);
		if (life)
			ft_print(philo, "is thinking");
		still_breathing(philo, &life);
	}
	return (NULL);
}
