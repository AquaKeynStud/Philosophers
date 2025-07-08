/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:10:28 by arocca            #+#    #+#             */
/*   Updated: 2025/07/08 16:50:25 by arocca           ###   ########.fr       */
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
	int	max_meals;
	int	time_to_die;
	int	time_to_eat;
	int	philos_count;
	int	time_to_sleep;
}				t_params;

typedef struct s_philo
{
	int				id;
	int				meals;
	bool			is_eating;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*monitoring;
}				t_philo;

typedef struct s_data
{
	bool			stop;
	long			start;
	pthread_mutex_t	state;
	struct s_params	params;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printer;
	struct s_philo	*philos;
}				t_data;

/* -- Functions -- */
long	get_time(void);
void	ms_wait(long ms);
void	*routine(void *arg);
void	cleanup(t_data *data);
bool	stopped(t_data *data);
void	*monitoring(void *arg);
void	exit_err(char *message);
long	timestamp(t_data *data);
int		ft_atoi(const char *nptr);
size_t	ft_strlen(const char *str);
int		start_philosophers(t_data *data);
void	wait_all(t_data *data, pthread_t monitor);
int		start_monitor(t_data *data, pthread_t *monitor_thread);
void	print(t_data *data, t_philo *philo, long ts, char *msg);

#endif