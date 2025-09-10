/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:46:17 by arocca            #+#    #+#             */
/*   Updated: 2025/09/10 17:28:27 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>
#include <stdlib.h>

int	usage_error(char **argv)
{
	write(2, "\n\e[1;31m🍁 Usage: ", 21);
	write(2, argv[0], ft_strlen(argv[0]));
	write(2, " [nb_philos] [time to die] [time to eat]", 40);
	write(2, " [time to sleep] (optionnal: quota) 🍁\n\n\e[0m", 47);
	return (1);
}

void	init_params(t_params *params, char **argv)
{
	int	i;
	int	convert;

	i = 1;
	params->max_meals = 0;
	while (i < 6 && argv[i])
	{
		convert = ft_atoi(argv[i]);
		if (convert <= 0 && i != 1)
			exit_err("\nAll parameters must be positive numbers\n");
		else if (i == 1 && (convert <= 0 || convert > 200))
			exit_err("\nYou can only launch between 1 and 200 philosphers\n");
		if (i == 1)
			params->nb_philo = convert;
		else if (i == 2)
			params->time_to_die = convert;
		else if (i == 3)
			params->time_to_eat = convert;
		else if (i == 4)
			params->time_to_sleep = convert;
		else if (i == 5 && argv[i])
			params->max_meals = convert;
		i++;
	}
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	exit_err(char *message)
{
	write(2, "\e[1m\e[31m", 9);
	write(2, message, ft_strlen(message));
	write(2, "\n\e[0m", 5);
	exit(1);
}

int	ft_atoi(const char *nptr)
{
	unsigned long long	res;
	int					sign;

	res = 0;
	sign = 1;
	if (!nptr || !*nptr)
		return (0);
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign *= -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		res = res * 10 + (*nptr++ - '0');
		if (res > LLONG_MAX && sign < 0)
			return (0);
		if (res > LLONG_MAX)
			return (-1);
	}
	return (((int)res) * sign);
}
