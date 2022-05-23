/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 14:46:03 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/23 21:35:12 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	check_digit(char *str)
{
	int	i;

	i = 0;

	while (str[i])
	{
		if (str[i] < 48 || str[i] > 57)
			return (-1);
		i++;
	}
	return (0);
}

int	check_args(char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (check_digit(av[i]))
			return (2);
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	int	ret;
	t_data	data;

	ret = check_args(av);
//	printf("%d\n", ret);
	if ((ac != 5 && ac != 6) || ret != 1)
		return (ft_return(ret, &data));
	ret = set_arg(&data, av, ac);
	if (ret)
		return (ft_return(ret, &data));
	ret = philosophers(&data);
	return (ft_return(ret, &data));
}
