
#include "../philosophers.h"

unsigned int	ft_atoi(char *nptr)
{
	int	i;
	unsigned long int	res;

	i = 0;
	res = 0;
	if (nptr[i] == '+')
		i++;
	while (nptr[i] > 47 && nptr[i] < 58)
	{
		res = res * 10 + (nptr[i] - 48);
		i++;
	}
	if (res > 4294967295)
		return (-1);
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

void	*ft_calloc(int nmem)
{
	int	i;
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
	return ((void*)str);
}
