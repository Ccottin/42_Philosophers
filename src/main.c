
#include "../philosophers.h"

void	set_arg(t_data *data, char **av)
{
	data->nb_p = ft_atoi(av[1]);
	data->t_t_d = ft_atoi(av[2]);
	data->t_t_e = ft_atoi(av[3]);
	data->t_t_s = ft_atoi(av[4]);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 4 || ac != 5 /*|| check_args != 0*/)
		return (ft_return(0)) ;
	ft_set_arg(&data, av);
	Philosophers(&data);	
}
