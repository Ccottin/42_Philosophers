
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

int	get_len(size_t nb)
{
	int	ret;

	ret = 0;
	while (nb > 9)
	{
		nb = nb / 10;
		ret++;
	}
	return (ret + 1);
}

char	*ft_itoa(size_t nb)
{
	char	*ret;
	int	size;

	size = get_len(nb);
	ret = ft_calloc(size + 1);
	if (!ret)
		return (NULL);
	while (size > 0)
	{
		size--;
		ret[size] = (nb % 10) + 48;
		nb = nb / 10;
	}
	return (ret);
}

void	ft_concat2(char **ret, char *s3, int i)
{
	int	j;

	j = 0;
	(*ret)[i++] = ' ';
	while (s3[j])
	{
		(*ret)[i] = s3[j];
		j++;
		i++;
	}
	(*ret)[i++] = '\n';
}

char	*ft_concat(char *s1, char *s2, char *s3)
{
	int	i;
	int	j;
	char	*ret;

	i = 0;
	j = 0;
	ret = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 4);
	if (!ret)
		return (NULL);
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	ret[i++] = ' ';
	while (s2[j])
	{
		ret[i] = s2[j];
		i++;
		j++;
	}
	ft_concat2(&ret, s3, i);
	free(s1);
	free(s2);
	return (ret);
}
