/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:10:28 by arocca            #+#    #+#             */
/*   Updated: 2025/08/02 14:49:56 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* -- Includes -- */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <pthread.h>

/* -- Structures -- */
typedef struct s_params
{
	long			max_meals;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	philos_count;
	unsigned long	time_to_sleep;
}				t_params;

typedef struct s_philo
{
	int				id;
	long			meals;
	pthread_t		thread;
	unsigned long	last_meal;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	*left_fork;
	struct s_data	*monitoring;
	pthread_mutex_t	*right_fork;
}				t_philo;

typedef struct s_data
{
	bool			stop;
	unsigned long	start;
	unsigned long	quota;
	pthread_mutex_t	state;
	struct s_params	params;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printer;
	struct s_philo	*philos;
}				t_data;

/* -- Functions -- */
unsigned long	get_time(void);
void			ms_wait(long ms);
void			*routine(void *arg);
bool			stopped(t_data *data);
void			cleanup(t_data *data);
void			*monitoring(void *arg);
void			exit_err(char *message);
int				usage_error(char **argv);
int				ft_atoi(const char *nptr);
size_t			ft_strlen(const char *str);
long			timestamp(unsigned long start);
int				start_philosophers(t_data *data);
void			wait_all(t_data *data, pthread_t monitor);
void			init_params(t_params *params, char **argv);
void			print(t_data *data, t_philo *philo, char *msg);
int				start_monitor(t_data *data, pthread_t *monitor_thread);

void	ms_waitb(long ms);

#endif