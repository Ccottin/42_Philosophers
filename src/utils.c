/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 20:16:34 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/24 15:04:47 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

unsigned int	ft_atoi(char *nptr)
{
	unsigned long int	res;
	unsigned int		i;

	i = 0;
	res = 0;
	if (nptr[i] == '+')
		i++;
	while (nptr[i] > 47 && nptr[i] < 58)
	{
		res = res * 10 + (nptr[i] - 48);
		i++;
	}
	if (res >= 429496)
		return (429496);
	return ((unsigned int)res);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == ft_strlen(s1) && i == ft_strlen(s2))
		return (0);
	return (1);
}

void	*ft_calloc(int nmem)
{
	int		i;
	char	*str;

	str = malloc(nmem);
	if (!str)
		return (NULL);
	i = 0;
	while (i < nmem)
	{
		str[i] = 0;
		i++;
	}
	return ((void *)str);
}
