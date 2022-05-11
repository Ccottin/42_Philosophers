/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 14:46:03 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/11 14:46:06 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../philosophers.h"

int	set_arg(t_data *data, char **av)
{
	t_philo		*philo;

	data->nb_p = ft_atoi(av[1]);
	data->t_t_d = ft_atoi(av[2]);
	data->t_t_e = ft_atoi(av[3]);
	data->t_t_s = ft_atoi(av[4]);
	philo = ft_calloc(sizeof(t_philo) * data->nb_p + 1);
	if (!philo)
		return (-1);
	data->philo = philo;
	data->l_data = ft_calloc(sizeof(pthread_mutex_t));
	data->printf = ft_calloc(sizeof(pthread_mutex_t));
	if (!data->printf || !data->l_data)
		return (-1);
	pthread_mutex_init(data->l_data, NULL);
	pthread_mutex_init(data->printf, NULL);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6 /*|| check_args != 0*/)
		return (ft_return(0, &data)) ;
	if (set_arg(&data, av) == -1)
		return (-1);
	Philosophers(&data);
	ft_return(1, &data);
}
