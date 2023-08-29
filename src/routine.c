/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/29 00:32:56 by niceguy          ###   ########.fr       */
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

static bool	can_pickup(t_philo *philo)
{
	bool	can;

	pthread_mutex_lock(philo->pickup);
	can = philo->forks[0]->check && philo->forks[1]->check;
	if (can)
	{
		philo->forks[0]->check = false;
		philo->forks[1]->check = false;
	}
	pthread_mutex_unlock(philo->pickup);
	if (can)
	{
		pthread_mutex_lock(&philo->forks[0]->mutex);
		ph_print(philo, MSG_FORK, get_time(philo->rules.start));
		pthread_mutex_lock(&philo->forks[1]->mutex);
		ph_print(philo, MSG_FORK, get_time(philo->rules.start));
	}
	return (can);
}

static void	eat(t_philo *philo)
{
	while (!can_pickup(philo))
	{
		if (!ph_is_alive(philo))
			return;
	}
	philo->last_meal = get_time(philo->rules.start);
	philo->num_meals++;
	ph_print(philo, MSG_EAT, get_time(philo->rules.start));
	ph_sleep(philo, philo->rules.time_to_eat);
	pthread_mutex_lock(philo->pickup);
	philo->forks[0]->check = true;
	philo->forks[1]->check = true;
	pthread_mutex_unlock(philo->pickup);
	pthread_mutex_unlock(&philo->forks[0]->mutex);
	pthread_mutex_unlock(&philo->forks[1]->mutex);
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
	{
		ph_print(philo, MSG_FORK, get_time(philo->rules.start));
		ph_sleep(philo, philo->rules.time_to_die);
		ph_print(philo, MSG_DIED, get_time(philo->rules.start));
		return (NULL);
	}
	while (ph_is_alive(philo))
	{
		eat(philo);
		if (!ph_is_alive(philo) || is_satisfied(philo))
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
