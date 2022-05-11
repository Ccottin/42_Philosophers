/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 18:16:15 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/11 14:31:54 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_print(t_philo *philo, char *str)
{
	size_t	time;

	if (pthread_mutex_lock(&(philo->life)))
		return (-1);
	if (!philo->is_alive && ft_strcmp("has died", str))
	{
		if (pthread_mutex_unlock(&(philo->life)))
			return (-1);
		return (0);
	}
	if (pthread_mutex_unlock(&(philo->life)))
		return (-1);
	if (get_time(&time, &(philo->b_time)) == -1)
		return (-1);
	if (pthread_mutex_lock(philo->printf))
		return (-1);
	printf("%lu %u %s\n", time, philo->nb, str);
	if (pthread_mutex_unlock(philo->printf))
		return (-1);
	return (0);
}

int	take_fork_1(t_philo *philo, unsigned int v)
{
	if (pthread_mutex_lock(philo->fork1_m))
		return (-1);
	if (v == 0)
		*philo->fork1 = 0;
	if (v != 0)
		*philo->fork1 = philo->nb;
	if (pthread_mutex_unlock(philo->fork1_m))
		return (-1);
	if (v != 0 && ft_print(philo, "has taken a fork") == -1)
		return (-1);
	return (0);
}

int	take_fork(t_philo *philo, unsigned int v)
{
	if (pthread_mutex_lock(&(philo->fork_m)))
		return (-1);
	if (v == 0)
		philo->fork = 0;
	if (v != 0)
		philo->fork = philo->nb;
	if (pthread_mutex_unlock(&(philo->fork_m)))
		return (-1);
	if (v != 0 && ft_print(philo, "has taken a fork") == -1)
		return (-1);
	return (0);
}

int	eat_last(t_philo *philo)
{
	if (take_fork_1(philo, philo->nb) == -1)
		return (-1);
	if (take_fork(philo, philo->nb) == -1)
		return (-1);
	if (ft_print(philo, "is eating") == -1)
		return (-1);
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	philo->p_time = philo->p_time + philo->t_t_d;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	usleep(philo->t_t_e * 1000);
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	philo->p_time = philo->p_time + philo->t_t_d;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	if (take_fork_1(philo, 0) == -1)
		return (-1);
	if (take_fork(philo, 0) == -1)
		return (-1);
	return (0);
}

int	eat(t_philo *philo)
{
	if (take_fork(philo, philo->nb) == -1)
		return (-1);
	if (take_fork_1(philo, philo->nb) == -1)
		return (-1);
	if (ft_print(philo, "is eating") == -1)
		return (-1);
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	philo->p_time = philo->p_time + philo->t_t_d;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	usleep(philo->t_t_e * 1000);
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	philo->p_time = philo->p_time + philo->t_t_d;
	if (pthread_mutex_unlock(&(philo->time_m)))
	if (take_fork(philo, 0) == -1)
		return (-1);
	if (take_fork_1(philo, 0) == -1)
		return (-1);
	return (0);
}

int	f_sleep(t_philo *philo)
{
	if (ft_print(philo, "is sleeping") == -1)
		return (-1);
	usleep(philo->t_t_s * 1000);
	return (0);
}

int	still_breathing(t_philo *philo, int *life)
{
	if (pthread_mutex_lock(&(philo->life)))
		return (-1);
	if (!philo->is_alive)
		*life = 0;
	if (pthread_mutex_unlock(&(philo->life)))
		return (-1);
	return (0);
}

void	*alive(void *ptr)
{
	t_philo	*philo;
	int	ret;
	int	life;
	
	philo = (t_philo*)ptr;
	life = 1;
	while (life)
	{
	//trouver le moyen de poser un truc pour checker si il peut prendre les fourchettes ou pqs
		still_breathing(philo, &life);
		if (life && philo->nb == philo->nb_p)
			ret = eat_last(philo);
		else if (life && philo->nb != philo->nb_p)
			ret = eat(philo);
		if (ret == -1)
			return ((void*)-1);
		still_breathing(philo, &ret);
		if (life && f_sleep(philo) == -1)
			return ((void*)-1);
		still_breathing(philo, &ret);
		if (life && ft_print(philo, "is thinking") == -1)
			return ((void*)-1);
	}
	return (NULL);
}

int	get_time(size_t *time, size_t *b_time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	if (b_time == NULL)
		*time = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
	else
		*time = ((tv.tv_sec * 1000000 + tv.tv_usec) / 1000) - *b_time;
	return (0);
}

void	init_philo(unsigned int nb, t_data *data)
{
	t_philo	philo;

	get_time(&(data->b_time), NULL);
	philo.nb = nb + 1;
	philo.t_t_d = data->t_t_d;
	philo.t_t_e = data->t_t_e;
	philo.t_t_s = data->t_t_s;
	philo.nb_p = data->nb_p;
	philo.printf = data->printf;
	philo.is_alive = 1;
	philo.fork = 0;
	pthread_mutex_init(&(philo.fork_m), NULL);
	pthread_mutex_init(&(philo.life), NULL);
	pthread_mutex_init(&(philo.time_m), NULL);
	if (nb == data->nb_p - 1)
	{
		philo.fork1 = &(data->philo[0].fork);
		philo.fork1_m = &(data->philo[0].fork_m);
	}
	else
	{
		philo.fork1 = &(data->philo[nb + 1].fork);
		philo.fork1_m = &(data->philo[nb + 1].fork_m);
	}
	philo.b_time = data->b_time;
	get_time(&philo.p_time, &(data->b_time));
	pthread_mutex_lock(data->l_data);
	data->philo[nb] = philo;
	pthread_mutex_unlock(data->l_data);
}

int	kill_em_all(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nb_p)
	{
		if (pthread_mutex_lock(&(data->philo[i].life)))
			return (-1);
		data->philo[i].is_alive = 0;
		if (pthread_mutex_unlock(&(data->philo[i].life)))
			return (-1);
		i++;
	}
	return (0);
}

int	check_alive(t_data *data)
{
	unsigned int	i;
	size_t		time;

	i = 0;
	while (7)
	{	if (get_time(&time, &(data->b_time)) == -1)
			return (-1);
		if (pthread_mutex_lock(&(data->philo[i].time_m)))
			return (-1);
		if (data->philo[i].p_time + data->t_t_d <= time)
		{
			ft_print(&(data->philo[i]), "timestamp");
			kill_em_all(data);
			ft_print(&(data->philo[i]), "has died");
			if (pthread_mutex_unlock(&(data->philo[i].time_m)))
				return (-1);
			return (0);
		}
		if (pthread_mutex_unlock(&(data->philo[i].time_m)))
			return (-1);
		i++;
		if (i == data->nb_p)
			i = 0;
	}
	return (0);
}

int	Philosophers(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nb_p)
	{
		init_philo(i, data);
		i++;
	}
	i = 0;
	while (i < data->nb_p)
	{
		if (pthread_create(&(data->philo[i].thread), NULL, &alive,(void*)&(data->philo[i])))
			return(-1);
		i++;
	}
	if (check_alive(data) == -1)
		ft_return(-1, data);
	else
	{
		i = 0;
		while (i < data->nb_p)
		{
			pthread_join(data->philo[i].thread, NULL);
			i++;
		}
	}
	return (0);
}
