/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:35:22 by arocca            #+#    #+#             */
/*   Updated: 2025/07/24 21:14:56 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "philo_bonus.h"

void	print_action(t_philo_bonus *philo, const char *msg)
{
	sem_wait(philo->monitor->write);
	printf("%lu %d %s\n", timestamp(philo->monitor->start), philo->id, msg);
	sem_post(philo->monitor->write);
}

static bool	init_philo(t_philo_bonus *philo, t_monitor *monitor, int i)
{
	char	*meal;

	philo->meals = 0;
	philo->id = i + 1;
	philo->monitor = monitor;
	philo->last_meal = monitor->start;
	meal = name_sem("/philo_meal_", philo->id);
	if (!init_sem(&philo->meal_lock, 1, meal))
	{
		free(meal);
		return (false);
	}
	free(meal);
	return (true);
}

static void	init_monitor(t_monitor *monitor, char **argv)
{
	int	i;
	int	nb_philos;

	i = 0;
	memset(monitor, 0, sizeof(t_monitor));
	monitor->start = get_time();
	monitor->params = malloc(sizeof(t_params));
	if (!monitor->params)
		exit_err("Failed to get program arguments\n");
	init_params(monitor->params, argv);
	nb_philos = monitor->params->philos_count;
	monitor->pids = malloc(sizeof(pid_t) * nb_philos);
	monitor->philos = malloc(sizeof(t_philo_bonus) * nb_philos);
	if (!monitor->pids || !monitor->philos
		|| !init_sem(&monitor->stop, 0, "/philo_stop")
		|| !init_sem(&monitor->quota, 0, "/philo_quota")
		|| !init_sem(&monitor->write, 1, "/philo_write")
		|| !init_sem(&monitor->forks, nb_philos, "/philo_forks"))
		clean_exit(monitor);
	while (i < nb_philos)
	{
		if (!init_philo(&monitor->philos[i], monitor, i))
			clean_exit(monitor);
		i++;
	}
}

int	main(int argc, char **argv)
{
	unsigned long	i;
	pid_t			pid;
	t_monitor		monitor;

	i = 0;
	if (argc != 5 && argc != 6)
		return (usage_error(argv));
	init_monitor(&monitor, argv);
	while (i < monitor.params->philos_count)
	{
		pid = fork();
		if (pid < 0)
			clean_exit(&monitor);
		else if (pid == 0)
			exit(philo_routine(&monitor.philos[i]));
		monitor.pids[i] = pid;
		i++;
	}
	monitoring_bonus(&monitor);
	return (0);
}
