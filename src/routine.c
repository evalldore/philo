/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/23 16:05:35 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	pickup_fork(t_philo *philo, uint32_t fork_index)
{
	pthread_mutex_lock(philo->forks[fork_index]);
	if (!ph_is_alive(philo))
	{
		pthread_mutex_unlock(philo->forks[fork_index]);
		return (false);
	}
	ph_print(philo, MSG_FORK, get_time(philo->rules.start));
	return (true);
}

static bool	eat(t_philo *philo)
{
	if (!pickup_fork(philo, 0))
		return (false);
	if (!pickup_fork(philo, 1))
	{
		pthread_mutex_unlock(philo->forks[0]);
		return (false);
	}
	ph_print(philo, MSG_EAT, get_time(philo->rules.start));
	pthread_mutex_lock(philo->death);
	philo->last_meal = get_time(philo->rules.start);
	philo->num_meals++;
	pthread_mutex_unlock(philo->death);
	ph_sleep(philo, philo->rules.time_to_eat);
	pthread_mutex_unlock(philo->forks[1]);
	pthread_mutex_unlock(philo->forks[0]);
	return (ph_is_alive(philo));
}

void	*ph_routine(void *ptr)
{
	t_philo				*philo;

	philo = ptr;
	if (philo->num_philos < 2)
		return (NULL);
	while (ph_is_alive(philo))
	{
		if (!eat(philo))
			return (NULL);
		ph_print(philo, MSG_SLEEP, get_time(philo->rules.start));
		if (!ph_sleep(philo, philo->rules.time_to_sleep))
			return (NULL);
		if (!ph_is_alive(philo))
			return (NULL);
		ph_print(philo, MSG_THINK, get_time(philo->rules.start));
	}
	return (NULL);
}
