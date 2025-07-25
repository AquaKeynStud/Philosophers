/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:02:13 by arocca            #+#    #+#             */
/*   Updated: 2025/07/22 20:56:34 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

void	print(t_data *data, t_philo *philo, char *msg)
{
	if (data->can)
		return ;
	pthread_mutex_lock(&data->printer);
	pthread_mutex_lock(&data->state);
	if (!data->stop)
		printf("%ld %d %s\n", timestamp(data->start), philo->id, msg);
	pthread_mutex_unlock(&data->state);
	pthread_mutex_unlock(&data->printer);
}

static void	init_philos(t_data *data)
{
	int	i;
	int	nb_philo;

	i = 0;
	nb_philo = data->params.philos_count;
	data->philos = malloc(sizeof(t_philo) * nb_philo);
	if (!data->philos)
	{
		free(data->forks);
		exit_err("Failed to allocate memory with malloc for the philosophers");
	}
	while (i < nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals = 0;
		data->philos[i].is_eating = false;
		data->philos[i].monitoring = data;
		data->philos[i].last_meal = data->start;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % nb_philo];
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL))
			exit_err("Init mutex meal failed");
		i++;
	}
}

static void	init_forks(t_data *data)
{
	unsigned long	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->params.philos_count);
	if (!data->forks)
		exit_err("Failed to allocate memory with malloc for the forks");
	while (i < data->params.philos_count)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			exit_err("Mutex init failed");
		i++;
	}
}

static void	init_data(t_data *data, char **argv)
{
	memset(data, 0, sizeof(t_data));
	init_params(&data->params, argv);
	if (data->params.philos_count == 1)
	{
		printf("0 1 has taken a fork\n");
		ms_wait(data->params.time_to_die);
		printf("%ld 1 died\n", data->params.time_to_die);
		return ;
	}
	if (pthread_mutex_init(&data->printer, NULL))
		exit_err("Printer mutex init failed");
	if (pthread_mutex_init(&data->state, NULL))
	{
		pthread_mutex_destroy(&data->printer);
		exit_err("State mutex init failed");
	}
	data->start = get_time() + 50;
	init_forks(data);
	init_philos(data);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor;

	if (argc != 5 && argc != 6)
		return (usage_error(argv));
	init_data(&data, argv);
	if (data.params.philos_count == 1)
		return (0);
	if (start_philosophers(&data))
		exit_err("Failed to create philosopher threads");
	if (start_monitor(&data, &monitor))
		exit_err("Failed to create monitoring thread");
	wait_all(&data, monitor);
	cleanup(&data);
	return (0);
}
