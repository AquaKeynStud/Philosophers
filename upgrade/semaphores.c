/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:07:30 by arocca            #+#    #+#             */
/*   Updated: 2025/09/10 17:22:38 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char	*ft_itoa(int n)
{
	int		len;
	long	copy;
	char	*result;

	len = (n < 0);
	copy = (long)(((long)n) * (1 - 2 * (n < 0)));
	while (copy && ++len)
		copy /= 10;
	result = (char *)malloc(len + 1 + (n == 0));
	if (!result)
		return (NULL);
	result[(n == 0) + (len--)] = '\0';
	if (n == 0)
		result[0] = '0';
	copy = (long)(((long)n) * (1 - 2 * (n < 0)));
	while (copy)
	{
		result[len--] = '0' + (copy % 10);
		copy /= 10;
	}
	if (len > -1 && n < 0)
		result[len--] = '-';
	return (result);
}

static char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*res;
	char			*start;

	if (!s1 || !s2)
		return (NULL);
	res = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!res)
		return (NULL);
	start = res;
	while (*s1)
		*res++ = *s1++;
	while (*s2)
		*res++ = *s2++;
	*res = '\0';
	return (start);
}

char	*name_sem(char *first, int philo)
{
	char	*result;
	char	*philo_char;

	if (!first || !*first)
		return (NULL);
	philo_char = ft_itoa(philo);
	if (!philo_char)
		return (NULL);
	result = ft_strjoin(first, philo_char);
	free(philo_char);
	if (!result)
		return (NULL);
	return (result);
}

bool	init_sem(sem_t **sem, int n, const char *unlink)
{
	sem_unlink(unlink);
	*sem = sem_open(unlink, (O_CREAT | O_EXCL), PERMS, n);
	if (*sem == SEM_FAILED)
	{
		printf("\e[1;31m🔥 Semaphore initialisation failed on: ");
		printf("%s 🔥\e[0m\n", unlink);
		return (false);
	}
	return (true);
}

void	clean_sem(sem_t *sem, const char *name)
{
	if (sem)
		sem_close(sem);
	if (name)
		sem_unlink(name);
}
