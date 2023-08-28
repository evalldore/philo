/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/28 18:26:04 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ph_sleep(t_philo *philo, uint64_t delay)
{
	uint64_t	start;

	start = get_time(philo->rules.start);
	while (ph_is_alive(philo) && (get_time(philo->rules.start) - start) < delay)
		usleep(50);
}

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

static void	eat(t_philo *philo)
{
	if (!pickup_fork(philo, 0))
		return ;
	if (!pickup_fork(philo, 1))
	{
		pthread_mutex_unlock(philo->forks[0]);
		return ;
	}
	philo->last_meal = get_time(philo->rules.start);
	philo->num_meals++;
	if (!ph_is_alive(philo))
		return ;
	ph_print(philo, MSG_EAT, get_time(philo->rules.start));
	ph_sleep(philo, philo->rules.time_to_eat);
	pthread_mutex_unlock(philo->forks[0]);
	pthread_mutex_unlock(philo->forks[1]);
}

static bool	is_satisfied(t_philo *philo)
{
	if (philo->rules.num_eats > 0 && philo->num_meals >= philo->rules.num_eats)
	{
		ph_print(philo, MSG_SATISFIED, get_time(philo->rules.start));
		return (true);
	}
	return (false);
}

void	*ph_routine(void *ptr)
{
	t_philo				*philo;

	philo = ptr;
	if (philo->num_philos < 2)
		return (NULL);
	while (ph_is_alive(philo))
	{
		eat(philo);
		if (is_satisfied(philo) || !ph_is_alive(philo))
			return (NULL);
		ph_print(philo, MSG_SLEEP, get_time(philo->rules.start));
		ph_sleep(philo, philo->rules.time_to_sleep);
		if (!ph_is_alive(philo))
			return (NULL);
		ph_print(philo, MSG_THINK, get_time(philo->rules.start));
	}
	return (NULL);
}
