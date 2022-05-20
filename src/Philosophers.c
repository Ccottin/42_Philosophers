/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 18:16:15 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/20 15:09:42 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

char	ft_print(t_philo *philo, char *str)
{
	unsigned int	time;

	if (get_time(&time, &(philo->b_time)) == -1)
		return (-1);
	if (pthread_mutex_lock(philo->printf))
		return (-1);
	printf("%u %u %s\n", time / 1000, philo->nb, str);
	if (pthread_mutex_unlock(philo->printf))
		return (-1);
	return (0);
}

char	take_fork_1(t_philo *philo, unsigned int v)
{
	if (v != 0 && ft_print(philo, "has taken a fork") == -1)
		return (-1);
	if (v == 0)
	{
		if (pthread_mutex_lock(philo->fork1_m))
			return (-1);
		*(philo->fork1) = 0;
		if (pthread_mutex_unlock(philo->fork1_m))
			return (-1);
	}
	return (0);
}

char	take_fork(t_philo *philo, unsigned int v)
{
	if (v != 0 && ft_print(philo, "has taken a fork") == -1)
		return (-1);
	if (v == 0)
	{
		if (pthread_mutex_lock(&(philo->fork_m)))
			return (-1);
		philo->fork = 0;
		if (pthread_mutex_unlock(&(philo->fork_m)))
			return (-1);
	}
	return (0);
}

char	set_new_time(t_philo *philo)
{
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	if (get_time(&(philo->p_time), &(philo->b_time)))
		return (-1);
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	return (0);
}

char	eat_last(t_philo *philo)
{
	if (take_fork_1(philo, philo->nb) == -1)
		return (-1);
	if (take_fork(philo, philo->nb) == -1)
		return (-1);
	if (ft_print(philo, "is eating") == -1)
		return (-1);
	if (set_new_time(philo))
		return (-1);
	usleep(philo->t_t_e);
	if (take_fork_1(philo, 0) == -1)
		return (-1);
	if (take_fork(philo, 0) == -1)
		return (-1);
	if (pthread_mutex_lock(&(philo->nb_eat_m)))
		return (-1);
	philo->nb_eat++;
	if (pthread_mutex_unlock(&(philo->nb_eat_m)))
		return (-1);
	return (0);
}

char	eat(t_philo *philo)
{
	if (take_fork(philo, philo->nb) == -1)
		return (-1);
	if (take_fork_1(philo, philo->nb) == -1)
		return (-1);
	if (ft_print(philo, "is eating") == -1)
		return (-1);
	if (set_new_time(philo))
		return (-1);
	usleep(philo->t_t_e);
	if (take_fork(philo, 0) == -1)
		return (-1);
	if (take_fork_1(philo, 0) == -1)
		return (-1);
	if (pthread_mutex_lock(&(philo->nb_eat_m)))
		return (-1);
	philo->nb_eat++;
	if (pthread_mutex_unlock(&(philo->nb_eat_m)))
		return (-1);
	return (0);
}

char	f_sleep(t_philo *philo)
{
	if (ft_print(philo, "is sleeping") == -1)
		return (-1);
	usleep(philo->t_t_s);
	return (0);
}

char	still_breathing(t_philo *philo, t_boolean *life)
{
	unsigned int	time;
	unsigned int	philo_time;
	
	if (pthread_mutex_lock(philo->is_alive_m))
		return (-1);
	if (*(philo->is_alive) == false)
		*life = false;
	if (pthread_mutex_unlock(philo->is_alive_m))
		return (-1);
	if (*life == false)
		return (0);
	if (get_time(&time, &(philo->b_time)) == -1)
		return (-1);
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	philo_time = philo->p_time;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	if (philo_time + philo->t_t_d <= time)
	{
		if (pthread_mutex_lock(philo->is_alive_m))
			return (-1);
		*(philo->is_alive) = 0;
		if (pthread_mutex_unlock(philo->is_alive_m))
			return (-1);
		if (pthread_mutex_lock(philo->dead_m))
			return (-1);
		*(philo->dead) = philo->nb;
		if (pthread_mutex_unlock(philo->dead_m))
			return (-1);
		*life = 0;
		usleep(1000);
		printf("%u %u has died\n",time / 1000, philo->nb);
	}
	return (0);
}

char	check_can_eat(t_philo *philo, char *ret)
{
	if (pthread_mutex_lock(&(philo->fork_m)))
		return (-1);
	*ret = philo->fork;
	if (pthread_mutex_unlock(&(philo->fork_m)))
		return (-1);
	return (0);
}

char	think(t_philo *philo)
{
	unsigned int	time;
	unsigned int	philo_time;
	unsigned int	must_wait;

	if (get_time(&time, &(philo->b_time)))
		return (-1);
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	philo_time = philo->p_time;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	if (pthread_mutex_lock(&(philo->politely_wait_m)))
		return (-1);
	must_wait = philo->politely_wait;
	if (pthread_mutex_unlock(&(philo->politely_wait_m)))
	if (philo_time + philo->t_t_d > must_wait)
		return (0);
	if (time < must_wait)
		usleep(must_wait - time);
	return (0);
}

void	*alive(void *ptr)
{
	t_philo	*philo;
	char	ret;
	t_boolean	life;

	philo = (t_philo*)ptr;
	life = true;
	if (philo->nb % 2 != 0)
		usleep(philo->t_t_e);
	still_breathing(philo, &life);
	while (life == true)
	{
		check_can_eat(philo, &ret);
		while (life && (unsigned int)ret != philo->nb)
		{	
			still_breathing(philo, &life);
		//	think(philo);	
			check_can_eat(philo, &ret);
		}
		if (life && philo->nb == philo->nb_p)
			ret = eat_last(philo);
		else if (life && philo->nb != philo->nb_p)
			ret = eat(philo);
		if (ret == -1)
			return ((void*)-1);
		still_breathing(philo, &life);
		if (life && f_sleep(philo) == -1)
			return ((void*)-1);
		still_breathing(philo, &life);
		if (life && ft_print(philo, "is thinking") == -1)
			return ((void*)-1);
	}
	return (NULL);
}

char	init_time(size_t *b_time)
{
	struct timeval	tv;
	
	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	*b_time = tv.tv_sec * 1000000 + tv.tv_usec;
	return (0);
}

char	get_time(unsigned int *time, size_t *b_time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	*time = (tv.tv_sec * 1000000 + tv.tv_usec) - *b_time;
	return (0);
}

//alrs comme ca on protege pas son init??
void	init_philo(unsigned int nb, t_data *data)
{
	t_philo	philo;

	philo.nb = nb + 1;
	philo.t_t_d = data->t_t_d;
	philo.t_t_e = data->t_t_e;
	philo.n_t_e = data->n_t_e;
	philo.nb_eat = 0;
	philo.t_t_s = data->t_t_s;
	philo.nb_p = data->nb_p;
	philo.printf = data->printf;
	philo.is_alive = &(data->is_alive);
	philo.is_alive_m = data->is_alive_m;
	philo.dead = &(data->dead);
	philo.dead_m = data->dead_m;
	philo.fork = 0;
	pthread_mutex_init(&(philo.politely_wait_m), NULL);
	pthread_mutex_init(&(philo.fork_m), NULL);
	pthread_mutex_init(&(philo.nb_eat_m), NULL);
	pthread_mutex_init(&(philo.time_m), NULL);
	if (data->nb_p != 1 && nb == data->nb_p - 1)
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
	get_time((&philo.p_time), &(data->b_time));
	pthread_mutex_lock(data->l_data);
	data->philo[nb] = philo;
	pthread_mutex_unlock(data->l_data);
}

char	set_fork(t_data *data)
{
	unsigned int i;

	i = 0;
	while (i < data->nb_p)
	{
		if (data->philo[i].nb % 2 == 0)
		{
			if (pthread_mutex_lock(&(data->philo[i].fork_m)))
				return (-1);
			data->philo[i].fork = data->philo[i].nb;
			if (pthread_mutex_unlock(&(data->philo[i].fork_m)))
				return (-1);
			if (pthread_mutex_lock(data->philo[i].fork1_m))
				return (-1);
			*(data->philo[i].fork1) = data->philo[i].nb;
			if (pthread_mutex_unlock(data->philo[i].fork1_m))
				return (-1);
		}
		i++;
	}
	return (0);
}

char	get_philo_time(t_philo *philo, unsigned int *ret)
{
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	*ret = philo->p_time;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	return (0);
}

char	must_wait(t_data *data, unsigned int i)
{
	unsigned int	philo_eating;

	if (get_time(&philo_eating, &(data->b_time)))
		return (-1);
	if (i == 0)
	{
		if (pthread_mutex_lock(&(data->philo[data->nb_p - 1].politely_wait_m)))
			return (-1);
		data->philo[data->nb_p - 1].politely_wait = philo_eating + data->t_t_e;
		if (pthread_mutex_unlock(&(data->philo[data->nb_p - 1].politely_wait_m)))
			return (-1);
	}
	else
	{
		if (pthread_mutex_lock(&(data->philo[i - 1].politely_wait_m)))
			return (-1);
		data->philo[i - 1].politely_wait = philo_eating + data->t_t_e;
		if (pthread_mutex_unlock(&(data->philo[i - 1].politely_wait_m)))
			return (-1);
	}
	if (i == data->nb_p - 1)
	{
		if (pthread_mutex_lock(&(data->philo[0].politely_wait_m)))
			return (-1);
		data->philo[0].politely_wait = philo_eating + data->t_t_e;
		if (pthread_mutex_unlock(&(data->philo[0].politely_wait_m)))
			return (-1);
 	}
	else
	{
		if (pthread_mutex_lock(&(data->philo[i + 1].politely_wait_m)))
			return (-1);
		data->philo[i + 1].politely_wait = philo_eating + data->t_t_e;
		if (pthread_mutex_unlock(&(data->philo[i + 1].politely_wait_m)))
			return (-1);
	}
	return (0);
}

char	philo_can_eat(t_data *data, unsigned int i)
{	
	if (pthread_mutex_lock(&(data->philo[i].fork_m)))
		return (-1);
	data->philo[i].fork = data->philo[i].nb;
	if (pthread_mutex_unlock(&(data->philo[i].fork_m)))
		return (-1);
	if (pthread_mutex_lock(data->philo[i].fork1_m))
		return (-1);
	*(data->philo[i].fork1) = data->philo[i].nb;
	if (pthread_mutex_unlock(data->philo[i].fork1_m))
		return (-1);
	if (must_wait(data, i))
		return (-1);
	return (0);
}

char	check_fellow(t_philo *philo, unsigned int time_i, unsigned int *ret)
{
	unsigned int	time_p;

	if (get_philo_time(philo, &time_p))
		return (-1);
	if (time_p < time_i)
		*ret = 1;
	else
		*ret = 0;
	return (0);
}

char	nagging_philo1(t_data *data, unsigned int i, unsigned int ret, unsigned int time_i)
{
	if (check_fellow(&(data->philo[i - 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (0);
	if (check_fellow(&(data->philo[0]), time_i, &ret))
		return (-1);
	if (ret)
		return (0);
	philo_can_eat(data, i);
	return (0);

}

char	nagging_philo(t_data *data, unsigned int i, unsigned int ret, unsigned int time_i)
{
	if (check_fellow(&(data->philo[data->nb_p - 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (0);
	if (check_fellow(&(data->philo[i + 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (0);
	philo_can_eat(data, i);
	return (0);
}

char	check_fork(t_data *data, unsigned int i, unsigned int *ret)
{
	if (pthread_mutex_lock(&(data->philo[i].fork_m)))
		return (-1);
	*ret = data->philo[i].fork;
	if (pthread_mutex_unlock(&(data->philo[i].fork_m)))
		return (-1);
	if (*ret != 0)
		return (0);
	if (pthread_mutex_lock(data->philo[i].fork1_m))
		return (-1);
	*ret = *(data->philo[i].fork1);
	if (pthread_mutex_unlock(data->philo[i].fork1_m))
		return (-1);
	return (0);
}

char	spaghettis(t_data *data, unsigned int i)
{
	unsigned int	ret;
	unsigned int	time_i;

	if (check_fork(data, i, &ret) == -1)
		return (-1);
	if (ret != 0)
		return (0);
	if (get_philo_time(&(data->philo[i]), &time_i))
		return (-1);
	if (i == 0)
		return (nagging_philo(data, i, ret, time_i));
	if (i == data->nb_p - 1)
		return (nagging_philo1(data, i, ret, time_i));
	if (check_fellow(&(data->philo[i - 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (0);
	if (check_fellow(&(data->philo[i + 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (0);
	philo_can_eat(data, i);
	return (0);
}

char	check_nte(t_data *data, unsigned int i, unsigned int *check_meal)
{
	unsigned int	temp;

	if (pthread_mutex_lock(&(data->philo[i].nb_eat_m)))
		return (-1);
	temp = data->philo[i].nb_eat;
	if (pthread_mutex_unlock(&(data->philo[i].nb_eat_m)))
		return (-1);
	if (temp >= data->n_t_e)
		*check_meal = *check_meal + 1;
	else
		*check_meal = 0;
	return (0);
}

char	check_alive(t_data *data)
{
	unsigned int	i;
	unsigned int	check_meal;

	i = 0;
	check_meal = 0;
	while (7)
	{	
		if (pthread_mutex_lock(data->is_alive_m))
			return (-1);
		if (data->is_alive == 0)
		{
			if (pthread_mutex_unlock(data->is_alive_m))
				return (-1);
			return (0);
		}
		if (pthread_mutex_unlock(data->is_alive_m))
			return (-1);
		if (spaghettis(data, i))
			return (-1);
		if (data->n_t_e && check_nte(data, i, &check_meal))
			return (-1);
		if (check_meal == data->nb_p)
		{
			if (pthread_mutex_lock(data->is_alive_m))
				return (-1);
			data->is_alive = 0;
			if (pthread_mutex_unlock(data->is_alive_m))
				return (-1);
			return (0);
		}
		i++;
		if (i == data->nb_p)
			i = 0;
	}
	return (0);
}

int	Philosophers(t_data *data)
{
	unsigned int	i;
	unsigned int	time;

	i = 0;
	printf("%lu\n", sizeof(t_boolean));
	init_time(&(data->b_time));
	while (i < data->nb_p)
	{
		init_philo(i, data);
		i++;
	}
	set_fork(data);
	i = 0;
	while (i < data->nb_p)
	{
		if (pthread_create(&(data->philo[i].thread), NULL, &alive,(void*)&(data->philo[i])))
			return(-1);
		i++;
	}
	if (check_alive(data) == -1)
		return (ft_return(-1, data));
	i = 0;
	while (i < data->nb_p)
	{
		if (pthread_join(data->philo[i].thread, NULL))
			return (-1);
		i++;
	}
	if (get_time(&time, &data->b_time) == -1)
		return (-1);
	return (0);
}
