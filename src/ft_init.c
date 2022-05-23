/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 20:17:20 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/23 20:22:51 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	set_calloc(t_data *data)
{
	t_philo	*philo;
	
	philo = ft_calloc(sizeof(t_philo) * data->nb_p + 1);
	if (!philo)
		return (-1);
	data->philo = philo;
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
int	set_arg(t_data *data, char **av, int ac)
{
	data->nb_p = ft_atoi(av[1]);
	if (data->nb_p == 0)
		return (3);
	data->t_t_d = ft_atoi(av[2]) * 1000;
	data->t_t_e = ft_atoi(av[3]) * 1000;
	data->t_t_s = ft_atoi(av[4]) * 1000;
	if (ac == 6)
	{
		data->n_t_e = ft_atoi(av[5]);
		if (data->n_t_e == 0)
			return (4);
	}
	if (ac == 5)
		data->n_t_e = 0;
	if (data->t_t_d >= 429496000 || data->t_t_e == 429496000 ||
		data->n_t_e  == 429496 || data->t_t_s == 429496000
		|| data->nb_p == 429496)
		return (5);
	return(set_calloc(data));
}
