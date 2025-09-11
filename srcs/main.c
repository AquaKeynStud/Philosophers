/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:02:13 by arocca            #+#    #+#             */
/*   Updated: 2025/09/11 09:57:47 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

void	print(t_data *data, t_philo *philo, char *msg)
{
	pthread_mutex_lock(&data->printer);
	if (!stopped(data))
	{
		printf("\e[1;96m-{%ld}-\e[0m", timestamp(data->start));
		printf("\e[1m %d %s\e[0m\n", philo->id, msg);
	}
	pthread_mutex_unlock(&data->printer);
}

static void	init_philos(t_data *data)
{
	int	i;
	int	nb_philo;

	i = 0;
	nb_philo = data->params.nb_philo;
	data->philos = malloc(sizeof(t_philo) * nb_philo);
	if (!data->philos)
	{
		clean_forks(data, 0);
		exit_err("💥 Failed to allocate memory for the philosophers table 💥");
	}
	while (i < nb_philo)
	{
		data->philos[i].meals = 0;
		data->philos[i].id = i + 1;
		data->philos[i].monitoring = data;
		data->philos[i].last_meal = data->start;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % nb_philo];
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL))
			clean_meals(data, i + 1);
		i++;
	}
}

static void	init_forks(t_data *data)
{
	unsigned long	i;

	i = 0;
	data->forks = malloc(sizeof(t_fork) * data->params.nb_philo);
	if (!data->forks)
		exit_err("💥 Failed to allocate memory for the forks table 💥");
	while (i < data->params.nb_philo)
	{
		data->forks[i].id = i + 1;
		data->forks[i].is_taken = false;
		if (pthread_mutex_init(&data->forks[i].fork, NULL))
		{
			clean_forks(data, i + 1);
			exit_err("🍴 Fork mutex init failed 🍴");
		}
		i++;
	}
}

static void	init_data(t_data *data, char **argv)
{
	memset(data, 0, sizeof(t_data));
	data->quota = 0;
	data->stop = false;
	init_params(&data->params, argv);
	if (data->params.nb_philo == 1)
		return (print_solo_report(data));
	if (pthread_mutex_init(&data->printer, NULL))
		exit_err("📝 Printer mutex init failed 📝");
	if (pthread_mutex_init(&data->state, NULL))
	{
		pthread_mutex_destroy(&data->printer);
		exit_err("🧬 State mutex init failed 🧬");
	}
	if (3 * data->params.nb_philo < 50)
		data->start = get_time() + 50;
	else
		data->start = get_time() + (3 * data->params.nb_philo);
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
	if (data.params.nb_philo == 1)
		return (0);
	if (start_philosophers(&data))
		exit_err("🔥 Failed to create philosopher threads 🔥");
	if (start_monitor(&data, &monitor))
		exit_err("🔥 Failed to create monitoring thread 🔥");
	wait_all(&data, monitor);
	cleanup(&data);
	return (0);
}
