
#include "../philosophers.h"

void	*eat(void *philo)
{
	t_philo	*philos;

	philos = (t_philo*)philo;
	pthread_mutex_lock(philos->printf);
	pthread_mutex_lock(&(philos->fork));
	printf("%d has taken a fork\n", philos->nb);
	pthread_mutex_lock(philos->fork1);
	printf("%d has taken a fork\n", philos->nb);
	pthread_mutex_unlock(philos->printf);
	usleep(philos->t_t_e);
	pthread_mutex_unlock(&(philos->fork));
	pthread_mutex_unlock(philos->fork1);
	return(NULL);
}

void	*alive(void *philo)
{
	eat(philo);
	return (NULL);
}

int	get_time(size_t *time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	*time = tv.tv_sec * 1000000 + tv.tv_usec;
	return (0);
}

void	init_philo(unsigned int nb, t_data *data)
{
	t_philo	philo;

	philo.b_time = data->b_time;
	philo.nb = nb + 1;
	philo.t_t_d = data->t_t_d;
	philo.t_t_e = data->t_t_e;
	philo.t_t_s = data->t_t_s;
	philo.printf = data->printf;
	pthread_mutex_init(&(philo.fork), NULL);
	if (nb == data->nb_p - 1)
		philo.fork1 = &(data->philo[0].fork);
	else
		philo.fork1 = &(data->philo[nb + 1].fork);
	data->philo[nb] = philo;
}

int	Philosophers(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nb_p)
	{
		init_philo(i, data);
		if (pthread_create(&(data->philo[i].thread), NULL, &alive,(void*)&(data->philo[i])))
			return(-1);
		i++;
	}
	i = 0;
	while (i < data->nb_p)
	{
		pthread_join(data->philo[i].thread, NULL);
		i++;
	}
	while (i < data->nb_p)
	{
		pthread_mutex_destroy(&(data->philo[i].fork));
		i++;
	}
	pthread_mutex_destroy(data->printf);
	return (0);
}
