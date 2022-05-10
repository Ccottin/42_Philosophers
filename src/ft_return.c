
#include "../philosophers.h"

void	free_all(t_data *data)
{
	unsigned int	i;
	while (i < data->nb_p)
	{
		pthread_mutex_destroy(&(data->philo[i].fork_m));
		i++;
	}
	pthread_mutex_destroy(data->printf);
	pthread_mutex_destroy(data->l_data);
	free(data->printf);
	free(data->l_data);
	free(data->philo);
}

int	ft_return(int mark, t_data *data)
{
	if (mark == 0)
	{
		printf("Arg requireds : [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] (opt.)[number_of_time_each_philosopher_must_eat]\n");
		return (0);
	}
	if (mark == 1)
		free_all(data);
	if (mark == -1)
	{
		write(1, "An error has occured.\n", 22);
		free_all(data);
	}
	return (0);
}
