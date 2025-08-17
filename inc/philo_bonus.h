/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:35:40 by arocca            #+#    #+#             */
/*   Updated: 2025/07/24 21:04:45 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

/* -- Includes -- */
# include "philo.h"
# include <fcntl.h>
# include <pthread.h>
# include <stdbool.h>
# include <semaphore.h>

/* -- Macros -- */
# define PERMS	0644

/* -- Structures -- */
typedef struct s_monitor	t_monitor;

typedef struct s_philo_bonus
{
	int				id;
	long			meals;
	t_monitor		*monitor;
	unsigned long	last_meal;
	sem_t			*meal_lock;
}			t_philo_bonus;

typedef struct s_monitor
{
	sem_t					*stop;
	pid_t					*pids;
	unsigned long			start;
	sem_t					*forks;
	sem_t					*write;
	sem_t					*quota;
	struct s_params			*params;
	struct s_philo_bonus	*philos;
}			t_monitor;

/* -- Functions -- */
char	*name_sem(char *first, int philo);
int		philo_routine(t_philo_bonus *philo);
void	monitoring_bonus(t_monitor *monitor);
void	clean_sem(sem_t *sem, const char *name);
void	clean_exit(t_monitor *monitor, int code);
bool	init_sem(sem_t **sem, int n, const char *unlink);
void	print_action(t_philo_bonus *philo, const char *msg);

#endif