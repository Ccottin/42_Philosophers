
#include "../philosophers.h"

int	set_arg(t_data *data, char **av)
{
	t_philo		*philo;
	pthread_mutex_t	*printf;

	data->nb_p = ft_atoi(av[1]);
	data->t_t_d = ft_atoi(av[2]);
	data->t_t_e = ft_atoi(av[3]);
	data->t_t_s = ft_atoi(av[4]);
	philo = ft_calloc(sizeof(t_philo) * data->nb_p + 1);
	if (!philo)
		return (-1);
	data->philo = philo;
	printf = ft_calloc(sizeof(pthread_mutex_t));
	if (!printf)
		return (-1);
	pthread_mutex_init(printf, NULL);
	data->printf = printf;
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 4 && ac != 5 /*|| check_args != 0*/)
		return (ft_return(0, &data)) ;
	if (set_arg(&data, av) == -1)
		return (-1);
	get_time(&(data.b_time));
	Philosophers(&data);
	ft_return(1, &data);
}
