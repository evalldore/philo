/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/27 06:37:12 by niceguy          ###   ########.fr       */
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

static void	eat(t_philo *philo)
{
	if (!pickup_fork(philo, 0))
		return ;
	if (!pickup_fork(philo, 1))
	{
		pthread_mutex_unlock(philo->forks[0]);
		return ;
	}
	pthread_mutex_lock(&philo->lock);
	philo->last_meal = get_time(philo->rules.start);
	philo->num_meals++;
	pthread_mutex_unlock(&philo->lock);
	if (!ph_is_alive(philo))
		return ;
	ph_print(philo, MSG_EAT, get_time(philo->rules.start));
	ph_sleep(philo, philo->rules.time_to_eat);
	pthread_mutex_unlock(philo->forks[0]);
	pthread_mutex_unlock(philo->forks[1]);
	return;
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
		if (!ph_is_alive(philo))
			return (NULL);
		ph_print(philo, MSG_SLEEP, get_time(philo->rules.start));
		ph_sleep(philo, philo->rules.time_to_sleep);
		if (!ph_is_alive(philo))
			return (NULL);
		ph_print(philo, MSG_THINK, get_time(philo->rules.start));
		usleep(150);
	}
	return (NULL);
}
