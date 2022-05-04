#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_data {
	unsigned int	nb_p;
	unsigned int	t_t_d;
	unsigned int	t_t_e;
	unsigned int	t_t_s;
}		t_data;

unsigned int	ft_atoi(char *nptr);
int	ft_return(int mark);

#endif
