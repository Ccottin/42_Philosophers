
#include "../philosophers.h"

void	free_all(t_data *data)
{
	free(data->philo);
	free(data->printf);
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
	return (0);
}
