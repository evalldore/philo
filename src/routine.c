/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/09/18 16:34:45 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ph_sleep(t_philo *philo, uint64_t delay)
{
	uint64_t	start;

	start = get_time(0);
	while ((get_time(0) - start) < delay)
	{
		if (!ph_is_alive(philo))
			return ;
		usleep(200);
	}
}

static bool	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->forks[0]);
	if (!ph_is_simulating(philo))
	{
		pthread_mutex_unlock(philo->forks[0]);
		return (false);
	}
	ph_print(philo, MSG_FORK);
	pthread_mutex_lock(philo->forks[1]);
	if (!ph_is_simulating(philo))
	{
		pthread_mutex_unlock(philo->forks[0]);
		pthread_mutex_unlock(philo->forks[1]);
		return (false);
	}
	ph_print(philo, MSG_FORK);
	pthread_mutex_lock(&philo->lock);
	philo->last_meal = get_time(philo->rules.start);
	philo->num_meals++;
	pthread_mutex_unlock(&philo->lock);
	ph_print(philo, MSG_EAT);
	ph_sleep(philo, philo->rules.time_to_eat);
	pthread_mutex_unlock(philo->forks[1]);
	pthread_mutex_unlock(philo->forks[0]);
	return (true);
}

static bool	is_satisfied(t_philo *philo)
{
	if (philo->rules.num_eats > 0 && philo->num_meals >= philo->rules.num_eats)
	{
		ph_print(philo, MSG_SATISFIED);
		return (true);
	}
	return (false);
}

void	*ph_routine(void *ptr)
{
	t_philo				*philo;

	philo = ptr;
	if ((philo->id % 2) == 0)
		ph_sleep(philo, philo->rules.time_to_eat);
	while (ph_is_simulating(philo))
	{
		if (!eat(philo))
			return (NULL);
		if (!ph_is_simulating(philo) || is_satisfied(philo))
			return (false);
		ph_print(philo, MSG_SLEEP);
		ph_sleep(philo, philo->rules.time_to_sleep);
		if (!ph_is_simulating(philo))
			return (NULL);
		ph_print(philo, MSG_THINK);
	}
	return (NULL);
}
