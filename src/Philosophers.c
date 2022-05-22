/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 18:16:15 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/22 21:28:37 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_print(t_philo *philo, char *str)
{
	unsigned int	time;

	if (pthread_mutex_lock(&(philo->is_alive_m)))
		return (-1);
	if (!philo->is_alive && ft_strcmp("has died", str))
	{
		if (pthread_mutex_unlock(&(philo->is_alive_m)))
			return (-1);
		return (0);
	}
	if (pthread_mutex_unlock(&(philo->is_alive_m)))
		return (-1);
	if (pthread_mutex_lock(philo->printf))
		return (-1);
	if (get_time(&time, &(philo->b_time)) == -1)
		return (-1);
	printf("%u %u %s\n", time / 1000, philo->nb, str);
	if (pthread_mutex_unlock(philo->printf))
		return (-1);
	return (0);
}

int	take_fork_1(t_philo *philo, char v)
{
	if (v == 1 && ft_print(philo, "has taken a fork"))
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

int	take_fork(t_philo *philo, char v)
{
	if (v == 1 && ft_print(philo, "has taken a fork"))
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

int	set_new_time(t_philo *philo)
{
	unsigned int	time;

	if (get_time(&time, &(philo->b_time)))
		return (-1);
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	philo->p_time = time;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	return (0);
}

int	eat_last(t_philo *philo)
{
	if (take_fork_1(philo, 1))
		return (-1);
	if (take_fork(philo, 1))
		return (-1);
	if (ft_print(philo, "is eating"))
		return (-1);
	if (set_new_time(philo))
		return (-1);
	usleep(philo->t_t_e);
	if (take_fork_1(philo, 0))
		return (-1);
	if (take_fork(philo, 0))
		return (-1);
	if (pthread_mutex_lock(&(philo->nb_eat_m)))
		return (-1);
	philo->nb_eat++;
	if (pthread_mutex_unlock(&(philo->nb_eat_m)))
		return (-1);
	return (0);
}

int	eat(t_philo *philo)
{
	if (take_fork(philo, 1))
		return (-1);
	if (take_fork_1(philo, 1))
		return (-1);
	if (ft_print(philo, "is eating"))
		return (-1);
	if (set_new_time(philo))
		return (-1);
	usleep(philo->t_t_e);
	if (take_fork(philo, 0))
		return (-1);
	if (take_fork_1(philo, 0))
		return (-1);
	if (pthread_mutex_lock(&(philo->nb_eat_m)))
		return (-1);
	philo->nb_eat++;
	if (pthread_mutex_unlock(&(philo->nb_eat_m)))
		return (-1);
	return (0);
}

int	still_breathing(t_philo *philo, char *life)
{
	unsigned int	time;
	unsigned int	philo_time;

	if (get_time(&time, &(philo->b_time)))
		return (-1);
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	philo_time = philo->p_time;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	if (philo_time + philo->t_t_d <= time)
	{
		if (pthread_mutex_lock(&(philo->is_alive_m)))
			return (-1);
		philo->is_alive = 0;
		if (pthread_mutex_unlock(&(philo->is_alive_m)))
			return (-1);
		*life = 0;
	}
	return (0);
}

int	f_sleep(t_philo *philo, char *life)
{
	unsigned int	sleep;

	if (ft_print(philo, "is sleeping"))
		return (-1);
	sleep = philo->t_t_s;
	while (life && sleep > 1000)
	{
		sleep = sleep / 1000;
		usleep(1000);
		still_breathing(philo, life);
	}
	if (sleep != philo->t_t_s)
		usleep(sleep);
	else
		usleep(philo->t_t_s);
	return (0);
}

int	check_can_eat(t_philo *philo, unsigned int *ret)
{
	if (pthread_mutex_lock(&(philo->fork_m)))
		return (-1);
	*ret = philo->fork;
	if (pthread_mutex_unlock(&(philo->fork_m)))
		return (-1);
	return (0);
}

int	think(t_philo *philo, char *life)
{
	unsigned int	fork;
	unsigned int	must_wait;

	if (check_can_eat(philo, &fork))
		return (-1);
	while (*life && fork != philo->nb)
	{
		if (pthread_mutex_lock(&(philo->politely_wait_m)))
			return (-1);
		must_wait = philo->politely_wait;
		if (pthread_mutex_unlock(&(philo->politely_wait_m)))
			return (-1);
		usleep(must_wait);
		still_breathing(philo, life);
		check_can_eat(philo, &fork);
	}
	return (0);
}

void	*alive(void *ptr)
{
	t_philo	*philo;
	char	ret;
	char	life;

	philo = (t_philo*)ptr;
	life = 1;
	if (philo->nb % 2 != 0)
	{
		if (life && ft_print(philo, "is thinking") == -1)
			return ((void*)-1);
		usleep(philo->t_t_e);
	}
	still_breathing(philo, &life);
	while (life)
	{
		still_breathing(philo, &life);
		if (think(philo, &life))
			return ((void*)-1);
		if (life && philo->nb == philo->nb_p)
			ret = eat_last(philo);
		else if (life && philo->nb != philo->nb_p)
			ret = eat(philo);
		if (ret == -1)
			return ((void*)-1);
		still_breathing(philo, &life);
		if (life && f_sleep(philo, &life) == -1)
			return ((void*)-1);
		still_breathing(philo, &life);
		if (life && ft_print(philo, "is thinking") == -1)
			return ((void*)-1);
	}
	return (NULL);
}

int	get_time(unsigned int *time, unsigned int *b_time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	if (b_time == NULL)
		*time = tv.tv_sec * 1000000 + tv.tv_usec;
	else
		*time = (tv.tv_sec * 1000000 + tv.tv_usec) - *b_time;
	return (0);
}

//alrs comme ca on protege pas son init??
void	init_philo(unsigned int nb, t_data *data)
{
	t_philo	philo;

	philo.nb = nb + 1;
	philo.t_t_d = data->t_t_d;
	if (data->t_t_d < data->t_t_s + data->t_t_e)
		philo.t_t_s = data->t_t_d - data->t_t_e;
	else
		philo.t_t_s = data->t_t_s;
	if (data->t_t_d < data->t_t_e)
		philo.t_t_e = data->t_t_d;
	else
		philo.t_t_e = data->t_t_e;
	philo.n_t_e = data->n_t_e;
	philo.nb_eat = 0;
	philo.nb_p = data->nb_p;
	philo.printf = data->printf;
	philo.is_alive = 1;
	philo.politely_wait = 0;
	philo.fork = 0;
	pthread_mutex_init(&(philo.politely_wait_m), NULL);
	pthread_mutex_init(&(philo.fork_m), NULL);
	pthread_mutex_init(&(philo.nb_eat_m), NULL);
	pthread_mutex_init(&(philo.is_alive_m), NULL);
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

int	set_fork(t_data *data)
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

int	kill_em_all(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nb_p)
	{
		if (pthread_mutex_lock(&(data->philo[i].is_alive_m)))
			return (-1);
		data->philo[i].is_alive = 0;
		if (pthread_mutex_unlock(&(data->philo[i].is_alive_m)))
			return (-1);
		i++;
	}
	return (0);
}

int	get_philo_time(t_philo *philo, unsigned int *ret)
{
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	*ret = philo->p_time;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	return (0);
}

int	must_wait(unsigned int last_meal, unsigned int t_t_e, unsigned int *wait_time, unsigned int time)
{
	unsigned int	shall_eat;

	shall_eat = last_meal + t_t_e;
	if (time > shall_eat)
	{
		*wait_time = 0;
		return (0);
	}
	*wait_time = shall_eat - time;
	return (0);
	}

int	must_wait_control(t_data *data, unsigned int i, unsigned int philo_eating, unsigned char mark)
{
	unsigned int	wait_time;
	unsigned int	philo_time;
	unsigned int	time;

	if (get_time(&time, &(data->b_time)))
		return (-1);
	if (mark == 1)
		wait_time = data->t_t_e;
	else
	{
		if (get_philo_time(&(data->philo[philo_eating - 1]), &philo_time))
			return (-1);
		if (must_wait(philo_time, data->t_t_e, &wait_time, time))
			return (-1);
	}
	if (get_philo_time(&(data->philo[i]), &philo_time))
		return (-1);
	if (wait_time + time >= data->t_t_d + philo_time && data->t_t_d + philo_time > time)
		wait_time = (data->t_t_d + philo_time) - time;
//	printf("%u %u  %u\n", i + 1, wait_time, time);
	if (pthread_mutex_lock(&(data->philo[i].politely_wait_m)))
		return (-1);
	data->philo[i].politely_wait = wait_time;
	if (pthread_mutex_unlock(&(data->philo[i].politely_wait_m)))
		return (-1);
	return (0);
}

int	philo_can_eat(t_data *data, unsigned int i)
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
	return (0);
}

int	check_fellow(t_philo *philo, unsigned int time_i, unsigned int *ret)
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

int	nagging_philo1(t_data *data, unsigned int i, unsigned int ret, unsigned int time_i)
{
	if (check_fellow(&(data->philo[i - 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait_control(data, i, 0, 1));
	if (check_fellow(&(data->philo[0]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait_control(data, i, 0, 1));
	philo_can_eat(data, i);
	return (0);

}

int	nagging_philo(t_data *data, unsigned int i, unsigned int ret, unsigned int time_i)
{
	if (check_fellow(&(data->philo[data->nb_p - 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait_control(data, i, 0, 1));
	if (check_fellow(&(data->philo[i + 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait_control(data, i, 0, 1));
	philo_can_eat(data, i);
	return (0);
}

int	check_fork(t_data *data, unsigned int i, unsigned int *ret)
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

int	spaghettis(t_data *data, unsigned int i)
{
	unsigned int	ret;
	unsigned int	time_i;

	if (check_fork(data, i, &ret) == -1)
		return (-1);
	if (ret != 0)
		return (must_wait_control(data, i, ret, 0));
	if (get_philo_time(&(data->philo[i]), &time_i))
		return (-1);
	if (i == 0)
		return (nagging_philo(data, i, ret, time_i));
	if (i == data->nb_p - 1)
		return (nagging_philo1(data, i, ret, time_i));
	if (check_fellow(&(data->philo[i - 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait_control(data, i, 0, 1));
	if (check_fellow(&(data->philo[i + 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait_control(data, i, 0, 1));
	philo_can_eat(data, i);
	return (0);
}

int	check_nte(t_data *data, unsigned int i, unsigned int *check_meal)
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

int	check_alive(t_data *data)
{
	unsigned int	i;
	unsigned char	philo_life;
	unsigned int	check_meal;

	i = 0;
	check_meal = 0;
	while (7)
	{	
		if (pthread_mutex_lock(&(data->philo[i].is_alive_m)))
			return (-1);
		philo_life = data->philo[i].is_alive;
		if (pthread_mutex_unlock(&(data->philo[i].is_alive_m)))
			return (-1);
		if (!philo_life)	
		{
			ft_print(&(data->philo[i]), "timestamp");
			data->dead = i + 1;
			kill_em_all(data);
			usleep(1000);
			ft_print(&(data->philo[i]), "has died");
			return (0);
		}
		if (spaghettis(data, i))
			return (-1);
		if (data->n_t_e && check_nte(data, i, &check_meal))
			return (-1);
		if (check_meal == data->nb_p)
		{
			kill_em_all(data);
			return (0);
		}
		i++;
		if (i == data->nb_p)
			i = 0;
	}
	return (0);
}

int	philosophers(t_data *data)
{
	unsigned int	i;

	i = 0;
	get_time(&(data->b_time), NULL);
	while (i < data->nb_p)
	{
		init_philo(i, data);
		i++;
	}
	set_fork(data);
	i = 0;
	while (i < data->nb_p)
	{
		if (pthread_create(&(data->philo[i].thread), NULL, &alive, (void*)&(data->philo[i])))
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
			if (pthread_join(data->philo[i].thread, NULL))
				return (-1);
			i++;
		}
	}
	return (0);
}
