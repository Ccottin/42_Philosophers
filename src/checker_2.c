/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 15:07:18 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/18 17:10:37 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
/*
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

int	get_philo_time(t_philo *philo, size_t *ret)
{
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	*ret = philo->p_time;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	return (0);
}

int	must_wait1(t_data *data, unsigned int i)
{
	if (pthread_mutex_lock(&(data->philo[i].politely_wait_m)))
		return (-1);
	data->philo[i].politely_wait = data->t_t_e;
	if (pthread_mutex_unlock(&(data->philo[i].politely_wait_m)))
		return (-1);
	return (0);
}

int	must_wait(t_data *data, unsigned int i, size_t philo_eating)
{
	size_t	p_eating_time;
	size_t	shall_eat;
	size_t	time;

	if (get_philo_time(&(data->philo[philo_eating - 1]), &p_eating_time))
		return (-1);
	shall_eat = p_eating_time + data->t_t_e;
	if (get_time(&time, &(data->b_time)))
		return (-1);
	if (time > shall_eat)
		return (0);
	p_eating_time = shall_eat - time;
	if (pthread_mutex_lock(&(data->philo[i].politely_wait_m)))
		return (-1);
	data->philo[i].politely_wait = p_eating_time;
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

int	check_fellow(t_philo *philo, size_t time_i, size_t *ret)
{
	size_t	time_p;

	if (get_philo_time(philo, &time_p))
		return (-1);
	if (time_p < time_i)
		*ret = 1;
	else
		*ret = 0;
	return (0);
}

int	nagging_philo1(t_data *data, unsigned int i, size_t ret, size_t time_i)
{
	if (check_fellow(&(data->philo[i - 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait1(data, i));
	if (check_fellow(&(data->philo[0]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait1(data, i));
	philo_can_eat(data, i);
	return (0);

}

int	nagging_philo(t_data *data, unsigned int i, size_t ret, size_t time_i)
{
	if (check_fellow(&(data->philo[data->nb_p - 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait1(data, i));
	if (check_fellow(&(data->philo[i + 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait1(data, i));
	philo_can_eat(data, i);
	return (0);
}

int	check_fork(t_data *data, unsigned int i, size_t *ret)
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

int	spaghettis(t_data *data, unsigned int i, size_t time_i)
{
	size_t	ret;

	if (check_fork(data, i, &ret) == -1)
		return (-1);
	if (ret != 0)
		return (must_wait(data, i, ret));
	if (i == 0)
		return (nagging_philo(data, i, ret, time_i));
	if (i == data->nb_p - 1)
		return (nagging_philo1(data, i, ret, time_i));
	if (check_fellow(&(data->philo[i - 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait1(data, i));
	if (check_fellow(&(data->philo[i + 1]), time_i, &ret))
		return (-1);
	if (ret)
		return (must_wait1(data, i));
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
*/
int	check_alive_2(t_data *data, t_checker *env)//modifiee
{
	size_t		time;
	size_t		philo_time;
	unsigned int	check_meal;
	int		i;

	check_meal = 0;
	while (7)
	{
		if (pthread_mutex_lock(&(env->j_m)))
			return (-1);
		i = env->j;
		if (pthread_mutex_unlock(env->&(j_m)))
			return (-1);
		if (i == -1)
			R
		if (get_time(&time, &(data->b_time)) == -1)
			return (-1);
		if (get_philo_time(&(data->philo[i]), &philo_time))
			return (-1);
		if (philo_time + data->t_t_d <= time)
		{
			ft_print(&(data->philo[i]), "timestamp");
			kill_em_all(data);
			usleep(1000);
			ft_print(&(data->philo[i]), "has died");
			return (0);
		}
		if (spaghettis(data, i, philo_time))
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

void	*second_checker(void *ptr)
{
	t_data	*env;

	env = (t_checker*)ptr;
	if (check_alive(env->data, env))
		return ((void*)-1);
	return (0);
}
