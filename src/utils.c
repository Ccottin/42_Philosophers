
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
	res = res * sign;
	if (res > 4294967295)
		return (-1);
	return (res);
}
