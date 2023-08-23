/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/23 14:54:17 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_simulate(t_philo *philo)
{
	pthread_mutex_lock(philo->death);
	if (!philo->simulating)
	{
		pthread_mutex_unlock(philo->death);
		return (false);
	}
	pthread_mutex_unlock(philo->death);
	return (true);
}

static void	ph_print(t_philo *philo, char *msg, uint64_t time)
{
	pthread_mutex_lock(philo->print);
	printf(msg, time, philo->id);
	pthread_mutex_unlock(philo->print);
}

static bool	pickup_fork(t_philo *philo, uint32_t fork_index)
{
	pthread_mutex_lock(philo->forks[fork_index]);
	if (!check_simulate(philo))
	{
		pthread_mutex_unlock(philo->forks[fork_index]);
		return (false);
	}
	ph_print(philo, MSG_FORK, get_time(philo->rules.start));
	return (true);
}

static bool	ph_sleep(t_philo *philo, uint64_t delay)
{
	uint64_t	start;

	start = get_time(philo->rules.start);
	while ((get_time(philo->rules.start) - start) < delay)
		usleep(1000);
	return (check_simulate(philo));
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
	return (check_simulate(philo));
}

void	*ph_routine(void *ptr)
{
	t_philo				*philo;

	philo = ptr;
	while (check_simulate(philo))
	{
		if (!eat(philo))
			return (NULL);
		ph_print(philo, MSG_SLEEP, get_time(philo->rules.start));
		if (!ph_sleep(philo, philo->rules.time_to_sleep))
			return (NULL);
		if (!check_simulate(philo))
			return (NULL);
		ph_print(philo, MSG_THINK, get_time(philo->rules.start));
	}
	return (NULL);
}
