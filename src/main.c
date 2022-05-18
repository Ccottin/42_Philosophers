/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 14:46:03 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/18 23:18:04 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../philosophers.h"

int	set_arg(t_data *data, char **av, int ac)
{
	t_philo		*philo;

	data->nb_p = ft_atoi(av[1]);
	data->t_t_d = ft_atoi(av[2]);
	data->t_t_e = ft_atoi(av[3]);
	data->t_t_s = ft_atoi(av[4]);
	if (ac == 6)
	{
		data->n_t_e = ft_atoi(av[5]);
		if (data->n_t_e == 0)
			return (1);
	}
	if (ac == 5)
		data->n_t_e = 0;
	philo = ft_calloc(sizeof(t_philo) * data->nb_p + 1);
	if (!philo)
		return (-1);
	data->philo = philo;
	data->dead = 0;
	data->l_data = ft_calloc(sizeof(pthread_mutex_t));
	data->printf = ft_calloc(sizeof(pthread_mutex_t));
	if (!data->printf || !data->l_data)
		return (-1);
	if (pthread_mutex_init(data->l_data, NULL))
		return (-1);
	if (pthread_mutex_init(data->printf, NULL))
		return (-1);
	return (0);
}

int	main(int ac, char **av)
{
	int	ret;
	t_data	data;

	if (ac != 5 && ac != 6 /*|| check_args != 0*/)
		return (ft_return(0, &data)) ;
	ret = set_arg(&data, av, ac);
	if (ret == -1)
		return (-1);
	if (ret == 1)
		return (0);
	Philosophers(&data);
	if (ft_return(1, &data))
		return (-1);
	return (0);
}
