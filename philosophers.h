#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_philo {
	size_t		p_time;
	size_t		b_time;
	unsigned int	nb;
	unsigned int	is_alive;
	unsigned int	t_t_d;
	unsigned int	t_t_e;
	unsigned int	t_t_s;
	unsigned int	nb_p;
	unsigned int	fork;
	unsigned int	*fork1;
	unsigned int	shall_eat;
	size_t		politely_wait;
	pthread_t	thread;
	pthread_mutex_t	politely_wait_m;
	pthread_mutex_t	shall_eat_m;
	pthread_mutex_t	fork_m;
	pthread_mutex_t	life;
	pthread_mutex_t	time_m;
	pthread_mutex_t	*fork1_m;
	pthread_mutex_t	*printf;
}		t_philo;

typedef struct s_data {
	size_t		b_time;
	unsigned int	t_t_d;
	unsigned int	t_t_e;
	unsigned int	t_t_s;
	unsigned int	nb_p;
	t_philo		*philo;
	pthread_mutex_t	*printf;
	pthread_mutex_t	*l_data;
}		t_data;

int		get_time(size_t *time, size_t *b_time);
unsigned int	ft_atoi(char *nptr);
int		ft_return(int mark, t_data *data);
void		*ft_calloc(int nmem);
int		Philosophers(t_data *data);
int		ft_strcmp(char *s1, char *s2);

#endif
