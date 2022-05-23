/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 18:16:15 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/23 21:33:29 by ccottin          ###   ########.fr       */
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

void	take_fork_1(t_philo *philo, char v)
{
	if (v == 1)
		ft_print(philo, "has taken a fork");
	if (v == 0)
	{
		pthread_mutex_lock(philo->fork1_m);
		*(philo->fork1) = 0;
		pthread_mutex_unlock(philo->fork1_m);
	}
}

void	take_fork(t_philo *philo, char v)
{
	if (v == 1)
		ft_print(philo, "has taken a fork");
	if (v == 0)
	{
		pthread_mutex_lock(&(philo->fork_m));
		philo->fork = 0;
		pthread_mutex_unlock(&(philo->fork_m));
	}
}

void	set_new_time(t_philo *philo)
{
	unsigned int	time;

	get_time(&time, &(philo->b_time));
	pthread_mutex_lock(&(philo->time_m));
	philo->p_time = time;
	pthread_mutex_unlock(&(philo->time_m));
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

void	f_eat(t_philo *philo, char *life)
{
	unsigned int	sleep;
	unsigned int	time;

	get_time(&time, &philo->b_time);
	sleep = philo->t_t_e + time;
	still_breathing(philo, life);
	while (*life && time < sleep)
	{
		usleep(500);
		still_breathing(philo, life);
		get_time(&time, &philo->b_time);
	}
}

void	eat_last(t_philo *philo, char *life)
{
	take_fork_1(philo, 1);
	take_fork(philo, 1);
	ft_print(philo, "is eating");
	set_new_time(philo);
	f_eat(philo, life);
	take_fork_1(philo, 0);
	take_fork(philo, 0);
	pthread_mutex_lock(&(philo->nb_eat_m));
	philo->nb_eat++;
	pthread_mutex_unlock(&(philo->nb_eat_m));
}

void	eat(t_philo *philo, char *life)
{
	take_fork(philo, 1);
	take_fork_1(philo, 1);
	ft_print(philo, "is eating");
	set_new_time(philo);
	f_eat(philo, life);
	take_fork(philo, 0);
	take_fork_1(philo, 0);
	pthread_mutex_lock(&(philo->nb_eat_m));
	philo->nb_eat++;
	pthread_mutex_unlock(&(philo->nb_eat_m));
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
		usleep(500);
		still_breathing(philo, life);
		get_time(&time, &philo->b_time);
	}
}

void	check_can_eat(t_philo *philo, unsigned int *ret)
{
	pthread_mutex_lock(&(philo->fork_m));
	*ret = philo->fork;
	pthread_mutex_unlock(&(philo->fork_m));
}

void	think(t_philo *philo, char *life)
{
	unsigned int	fork;

	check_can_eat(philo, &fork);
	while (*life && fork != philo->nb)
	{
		usleep(500);
		still_breathing(philo, life);
		check_can_eat(philo, &fork);
	}
}

void	*alive(void *ptr)
{
	t_philo	*philo;
	char	life;

	philo = (t_philo*)ptr;
	life = 1;
	if (philo->nb % 2 != 0)
		usleep(philo->t_t_e);
	still_breathing(philo, &life);
	while (life)
	{
		if (life)
			think(philo, &life);
		still_breathing(philo, &life);
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

//alrs comme ca on protege pas son init??
int	init_philo(unsigned int nb, t_data *data)
{
	t_philo	philo;

	philo.nb = nb + 1;
	philo.t_t_d = data->t_t_d;
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
	philo.fork = 0;
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
	get_time(&(philo.p_time), &(data->b_time));
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
	unsigned int i;

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

void	get_philo_time(t_philo *philo, unsigned int *ret)
{
	pthread_mutex_lock(&(philo->time_m));
	*ret = philo->p_time;
	pthread_mutex_unlock(&(philo->time_m));
}

void	philo_can_eat(t_data *data, unsigned int i)
{	
	pthread_mutex_lock(&(data->philo[i].fork_m));
	data->philo[i].fork = data->philo[i].nb;
	pthread_mutex_unlock(&(data->philo[i].fork_m));
	pthread_mutex_lock(data->philo[i].fork1_m);
	*(data->philo[i].fork1) = data->philo[i].nb;
	pthread_mutex_unlock(data->philo[i].fork1_m);
}

void	check_fellow(t_philo *philo, unsigned int time_i, unsigned int *ret)
{
	unsigned int	time_p;

	get_philo_time(philo, &time_p);
	if (time_p < time_i)
		*ret = 1;
	else
		*ret = 0;
}

void	nagging_philo1(t_data *data, unsigned int i, unsigned int ret, unsigned int time_i)
{
	check_fellow(&(data->philo[i - 1]), time_i, &ret);
	if (ret)
		return ;
	check_fellow(&(data->philo[0]), time_i, &ret);
	if (ret)
		return ;
	philo_can_eat(data, i);

}

void	nagging_philo(t_data *data, unsigned int i, unsigned int ret, unsigned int time_i)
{
	check_fellow(&(data->philo[data->nb_p - 1]), time_i, &ret);
	if (ret)
		return ;
	check_fellow(&(data->philo[i + 1]), time_i, &ret);
	if (ret)
		return ;
	philo_can_eat(data, i);
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

void	spaghettis(t_data *data, unsigned int i)
{
	unsigned int	ret;
	unsigned int	time_i;

	check_fork(data, i, &ret);
	if (ret != 0)
		return ;
	get_philo_time(&(data->philo[i]), &time_i);
	if (i == 0)
		return (nagging_philo(data, i, ret, time_i));
	if (i == data->nb_p - 1)
		return (nagging_philo1(data, i, ret, time_i));
	check_fellow(&(data->philo[i - 1]), time_i, &ret);
	if (ret)
		return ;
	check_fellow(&(data->philo[i + 1]), time_i, &ret);
	if (ret)
		return ;
	philo_can_eat(data, i);
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

void	check_alive(t_data *data)
{
	unsigned int	i;
	unsigned char	philo_life;
	unsigned int	check_meal;

	i = 0;
	check_meal = 0;
	while (7)
	{	
		pthread_mutex_lock(&(data->philo[i].is_alive_m));
		philo_life = data->philo[i].is_alive;
		pthread_mutex_unlock(&(data->philo[i].is_alive_m));
		if (!philo_life)	
		{
			kill_em_all(data);
			usleep(1000);
			ft_print(&(data->philo[i]), "died");
			return ;
		}
		spaghettis(data, i);
		if (data->n_t_e)
			check_nte(data, i, &check_meal);
		if (check_meal == data->nb_p)
		{
			kill_em_all(data);
			return ;
		}
		i++;
		if (i == data->nb_p)
			i = 0;
	}
}

int	philoloner(t_data *data)
{
	unsigned int	time;

	get_time(&time, &(data->b_time));
	printf("%u 1 is thinking\n", time / 1000);
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
		if (pthread_create(&(data->philo[i].thread), NULL, &alive, (void*)&(data->philo[i])))
			return(-1);
		i++;
	}
	check_alive(data);
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
