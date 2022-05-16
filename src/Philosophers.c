/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 18:16:15 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/11 20:17:57 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_print(t_philo *philo, char *str)
{
	size_t	time;

	if (get_time(&time, &(philo->b_time)) == -1)
		return (-1);
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
	if (pthread_mutex_lock(philo->printf))
		return (-1);
	printf("%lu %u %s\n", time, philo->nb, str);
	if (pthread_mutex_unlock(philo->printf))
		return (-1);
	return (0);
}

int	take_fork_1(t_philo *philo, unsigned int v)
{
	if (v != 0 && ft_print(philo, "has taken a fork") == -1)
		return (-1);
	if (pthread_mutex_lock(philo->fork1_m))
		return (-1);
	if (v == 0)
		*philo->fork1 = 0;
	if (v != 0)
		*philo->fork1 = philo->nb;
	if (pthread_mutex_unlock(philo->fork1_m))
		return (-1);
	return (0);
}

int	take_fork(t_philo *philo, unsigned int v)
{
	if (v != 0 && ft_print(philo, "has taken a fork") == -1)
		return (-1);
	if (pthread_mutex_lock(&(philo->fork_m)))
		return (-1);
	if (v == 0)
		philo->fork = 0;
	if (v != 0)
		philo->fork = philo->nb;
	if (pthread_mutex_unlock(&(philo->fork_m)))
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
	if (pthread_mutex_lock(&(philo->shall_eat_m)))
		return (-1);
	philo->shall_eat = 0;
	if (pthread_mutex_unlock(&(philo->shall_eat_m)))
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
	philo->p_time = philo->p_time + philo->t_t_e;
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
	if (pthread_mutex_lock(&(philo->shall_eat_m)))
		return (-1);
	philo->shall_eat = 0;
	if (pthread_mutex_unlock(&(philo->shall_eat_m)))
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
	size_t	meal;
	
	philo = (t_philo*)ptr;
	life = 1;
	meal = 0;
	if (philo->nb % 2 != 0)
	{
		if (life && ft_print(philo, "is thinking") == -1)
			return ((void*)-1);
		usleep(philo->t_t_e * 1000);
	}
	else
		meal = 1;
	while (life)
	{
		still_breathing(philo, &life);
		while (life && meal == 0)
		{	
			if (pthread_mutex_lock(&(philo->politely_wait_m)))
				return ((void*)-1);
			meal = philo->politely_wait;
			if (pthread_mutex_unlock(&(philo->politely_wait_m)))
				return ((void*)-1);
			usleep(meal);
			still_breathing(philo, &life);
			if (pthread_mutex_lock(&(philo->shall_eat_m)))
				return ((void*)-1);
			meal = philo->shall_eat;
			if (pthread_mutex_unlock(&(philo->shall_eat_m)))
				return ((void*)-1);
			still_breathing(philo, &life);
		}
	//	printf("philo = %u, sheat = %u\n", philo->nb, philo->shall_eat);
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

//alrs comme ca on protege pas son init??
void	init_philo(unsigned int nb, t_data *data)
{
	t_philo	philo;

	philo.nb = nb + 1;
	philo.t_t_d = data->t_t_d;
	philo.t_t_e = data->t_t_e;
	philo.t_t_s = data->t_t_s;
	philo.nb_p = data->nb_p;
	philo.printf = data->printf;
	philo.is_alive = 1;
	if (nb % 2 == 0)
		philo.shall_eat = 1;
	else
		philo.shall_eat = 0;
	philo.politely_wait = 0;
	philo.fork = 0;
	pthread_mutex_init(&(philo.shall_eat_m), NULL);
	pthread_mutex_init(&(philo.politely_wait_m), NULL);
	pthread_mutex_init(&(philo.fork_m), NULL);
	pthread_mutex_init(&(philo.life), NULL);
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

int	check_fork(t_data *data, unsigned int i, size_t *ret)
//si check fork revient 0 c libre sinon c id du philo qui mange
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

int	get_philo_time(t_philo *philo, size_t *ret)
{
	if (pthread_mutex_lock(&(philo->time_m)))
		return (-1);
	*ret = philo->p_time;
	if (pthread_mutex_unlock(&(philo->time_m)))
		return (-1);
	return (0);
}

int	must_wait(t_data *data, unsigned int philo_eating, size_t *ret, unsigned int i)
{
	size_t	time;

	if (pthread_mutex_lock(&(data->philo[philo_eating].time_m)))
		return (-1);
	*ret = data->philo[philo_eating].p_time;
	if (pthread_mutex_unlock(&(data->philo[philo_eating].time_m)))
		return (-1);
	if (get_time(&time, &(data->b_time)))
		return (-1);
	if (*ret < time)
	{
		if (pthread_mutex_lock(&(data->philo[i].politely_wait_m)))
			return (-1);
		data->philo[i].politely_wait = 0;
		if (pthread_mutex_unlock(&(data->philo[i].politely_wait_m)))
			return (-1);
	}
	else
	{
		if (pthread_mutex_lock(&(data->philo[i].politely_wait_m)))
			return (-1);
		data->philo[i].politely_wait = *ret - time;
		if (pthread_mutex_unlock(&(data->philo[i].politely_wait_m)))
			return (-1);
	}
	return (0);
}

int	check_fellow(t_philo *philo, int *temp, size_t time_i)
{//retourne moins un si philo + 1 doit attendre
	size_t	retemp;

	if (get_philo_time(philo, &retemp))
		return (-1);
	if (retemp > time_i)
		*temp = -1;
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

int	check_no_one(t_data *data, unsigned int i, size_t *temp)
{
	unsigned int	jpex;

	if (i == 0)
	{
		if (pthread_mutex_lock(&(data->philo[data->nb_p - 1].shall_eat_m)))
			return (-1);
		jpex = data->philo[data->nb_p - 1].shall_eat;
		if (pthread_mutex_unlock(&(data->philo[data->nb_p - 1].shall_eat_m)))
			return (-1);
	}
	else
	{
		if (pthread_mutex_lock(&(data->philo[i - 1].shall_eat_m)))
			return (-1);
		jpex = data->philo[i - 1].shall_eat;
		if (pthread_mutex_unlock(&(data->philo[i - 1].shall_eat_m)))
			return (-1);
	}
//	printf("jpex = %u, i = %u\n", jpex, i);
	if (jpex == 1)
		*temp = 1;
	else
		*temp = 0;
//	printf("%u, shall eat = %lu et avant = %u\n", data->philo[i].nb, *temp, jpex);
	return (0);
}

int	philo_can_eat(t_data *data, unsigned int i, size_t time_i)
{	
	size_t	temp;
	int	ret;

	check_no_one(data, i, &temp);
	if (temp == 1)
		return (must_wait1(data, i));
	if (i == data->nb_p - 1)
		ret = get_philo_time(&(data->philo[0]), &temp);
	else
		ret = get_philo_time(&(data->philo[i + 1]), &temp);
	if (ret)
		return (-1);
//	printf("%lu, %lu\n", temp, time_i);
	if (temp < time_i)
		return (must_wait1(data, i));
	if (i == 0)
		ret = get_philo_time(&(data->philo[data->nb_p -1]), &temp);
	else
		ret = get_philo_time(&(data->philo[i - 1]), &temp);
	if (ret)
		return (-1);
	if (temp < time_i)
		return (must_wait1(data, i));
	if (pthread_mutex_lock(&(data->philo[i].shall_eat_m)))
		return (-1);
	data->philo[i].shall_eat = 1;
	if (pthread_mutex_unlock(&(data->philo[i].shall_eat_m)))
		return (-1);
	return (0);
}

int	nagging_philo1(t_data *data, unsigned int i, size_t ret, size_t time_i)
{
	int	temp;

	temp = 0;
	if (ret != i - 1 && check_fellow(&(data->philo[i - 1]), &temp, time_i))
		return (-1);
	if (temp != -1)
		return (must_wait1(data, i));
	if (ret != 1 && check_fellow(&(data->philo[0]), &temp, time_i))
		return (-1);
	if (temp != -1)
		return (must_wait1(data, i));
	if (ret == 1)
		return (must_wait(data, 0, &ret, i));
	return (must_wait(data, ret - 1, &ret, i));
}

int	nagging_philo(t_data *data, unsigned int i, size_t ret, size_t time_i)
{
	int	temp;

	temp = 0;
	if (ret != data->nb_p && check_fellow(&(data->philo[data->nb_p - 1]), &temp, time_i))
		return (-1);
	if (temp != -1)
		return (must_wait1(data, i));
	if (ret != i + 1 && check_fellow(&(data->philo[i + 1]), &temp, time_i))
			return (-1);
	if (temp != -1)
		return (must_wait1(data, i));
	if (ret == data->nb_p)
		return (must_wait(data, data->nb_p - 1, &ret, i));
	return (must_wait(data, ret - 1, &ret, i));
}


int	spaghettis(t_data *data, unsigned int i)
{
	int	temp;
	size_t	ret;
	size_t	time_i;

	if (check_fork(data, i, &ret))
		return (-1);
//	printf("i = %u , fourchette = %ld\n", i + 1, ret);
	if (ret == data->philo[i].nb)
		return (0);
	if (get_philo_time(&(data->philo[i]), &time_i))
		return (-1);
	if (ret != 0)
	{
		if (i == 0)
			return (nagging_philo(data, i, ret, time_i));
		if (i == data->nb_p - 1)
			return (nagging_philo1(data, i, ret, time_i));
		/*on peut surement opti en enleveant la premier condition vu qu'ils sont déjà check au philo précédent*/
		temp = 0;
		if (ret != i - 1 && check_fellow(&(data->philo[i - 1]), &temp, time_i))
			return (-1);
		if (temp != -1)
			return (must_wait1(data, i));
		if (ret != i + 1 && check_fellow(&(data->philo[i + 1]), &temp, time_i))
			return (-1);
		if (temp != -1)
			return (must_wait1(data, i));
		return (must_wait(data, ret - 1, &ret, i));
	}
	philo_can_eat(data, i, time_i);
	return (0);
}

int	check_alive(t_data *data)
{
	unsigned int	i;
	size_t		time;

	i = 0;
	while (7)
	{
		if (get_time(&time, &(data->b_time)) == -1)
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
		if (spaghettis(data, i))
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
	get_time(&(data->b_time), NULL);
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
			if (pthread_join(data->philo[i].thread, NULL))
				return (-1);
			i++;
		}
	}
	return (0);
}
