/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 17:34:32 by ccottin           #+#    #+#             */
/*   Updated: 2022/05/24 18:16:13 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

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
	unsigned int	*fork1;
	unsigned char	is_alive;
	pthread_t		thread;
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
	char			ac;
	pthread_mutex_t	*printf;
	pthread_mutex_t	*l_data;
	t_philo			*philo;
}		t_data;

void			check_nte(t_data *data, unsigned int i,
					unsigned int *check_meal);
void			check_fork(t_data *data, unsigned int i, unsigned int *ret);
void			get_time(unsigned int *time, unsigned int *b_time);
void			philo_can_eat(t_data *data, unsigned int i);
void			still_breathing(t_philo *philo, char *life);
void			ft_print(t_philo *philo, char *str);
void			eat_last(t_philo *philo, char *life);
void			eat(t_philo *philo, char *life);
void			check_alive(t_data *data, unsigned int i);
void			*ft_calloc(int nmem);
void			*alive(void *ptr);

int				ft_return(int mark, t_data *data);
int				ft_strcmp(char *s1, char *s2);
int				philosophers(t_data *data);
int				set_calloc(t_data *data);
int				ft_init(t_data *data);

unsigned int	ft_atoi(char *nptr);

#endif
