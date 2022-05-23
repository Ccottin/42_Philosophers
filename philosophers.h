#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_philo {
	unsigned int	p_time;
	unsigned int	b_time;
	unsigned int	nb;
	unsigned int	nb_eat;
	unsigned int	t_t_d;
	unsigned int	t_t_e;
	unsigned int	n_t_e;
	unsigned int	t_t_s;
	unsigned int	nb_p;
	unsigned int	fork;
	unsigned char	is_alive;
	unsigned int	*fork1;
	pthread_t	thread;
	pthread_mutex_t	fork_m;
	pthread_mutex_t	nb_eat_m;
	pthread_mutex_t	is_alive_m;
	pthread_mutex_t	time_m;
	pthread_mutex_t	*fork1_m;
	pthread_mutex_t	*printf;
}		t_philo;

typedef struct s_data {
	unsigned int	b_time;
	unsigned int	t_t_d;
	unsigned int	t_t_e;
	unsigned int	n_t_e;
	unsigned int	t_t_s;
	unsigned int	nb_p;
	char		ac;
	t_philo		*philo;
	pthread_mutex_t	*printf;
	pthread_mutex_t	*l_data;
}		t_data;

void		get_time(unsigned int *time, unsigned int *b_time);
int		set_arg(t_data *data, char **av, int ac);
int		ft_return(int mark, t_data *data);
int		ft_strcmp(char *s1, char *s2);
int		philosophers(t_data *data);
unsigned int	ft_atoi(char *nptr);
void		*ft_calloc(int nmem);

#endif
