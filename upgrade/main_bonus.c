/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:35:22 by arocca            #+#    #+#             */
/*   Updated: 2025/09/11 10:38:21 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "philo_bonus.h"

void	print_action(t_philo_bonus *philo, const char *msg)
{
	if (!philo->active)
		return ;
	sem_wait(philo->monitor->write);
	printf("\e[1;34m-{%lu}-", timestamp(philo->monitor->start));
	printf("\e[0;1m %d %s\e[0m\n", philo->id, msg);
	sem_post(philo->monitor->write);
}

static bool	init_philo(t_philo_bonus *philo, t_monitor *monitor, int i)
{
	char	*meal;

	philo->meals = 0;
	philo->id = i + 1;
	philo->active = true;
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

static void	init_monitor(t_monitor *data, char **argv)
{
	int	i;
	int	n_philo;

	i = 0;
	memset(data, 0, sizeof(t_monitor));
	data->params = malloc(sizeof(t_params));
	if (!data->params)
		exit_err("\e[1;31m🍁 Failed to get program arguments 🍁\e[0m\n");
	init_params(data->params, argv);
	n_philo = data->params->nb_philo;
	data->start = get_time() + n_philo;
	data->pids = malloc(sizeof(pid_t) * n_philo);
	data->philos = malloc(sizeof(t_philo_bonus) * n_philo);
	if (!data->pids || !data->philos
		|| !init_sem(&data->stop, 0, "p_stop")
		|| !init_sem(&data->quota, 0, "p_quota")
		|| !init_sem(&data->write, 1, "p_write")
		|| !init_sem(&data->forks, n_philo, "p_forks")
		|| !init_sem(&data->limit, ((n_philo / 2) + (n_philo % 2)), "p_limit"))
		clean_exit(data, 1);
	while (i < n_philo)
		if (!init_philo(&data->philos[i], data, i))
			clean_exit(data, 1);
	else
		i++;
}

static int	handle_single_philo_bonus(t_monitor *monitor)
{
	printf("\e[1;34m-{0}-\e[0;1m 1 has taken a fork 🍴\e[0m\n");
	ms_wait(monitor->params->time_to_die);
	printf("\e[1;34m-{%ld}-", monitor->params->time_to_die);
	printf("\e[0;1m 1 died 🩻\e[0m\n");
	clean_exit(monitor, 0);
	return (0);
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
	monitor.active = true;
	if (monitor.params->nb_philo == 1)
		return (handle_single_philo_bonus(&monitor));
	while (i < monitor.params->nb_philo)
	{
		pid = fork();
		if (pid < 0)
			clean_exit(&monitor, 1);
		else if (pid == 0)
			exit(philo_routine(&monitor.philos[i]));
		monitor.pids[i] = pid;
		i++;
	}
	monitoring_bonus(&monitor);
	return (0);
}
