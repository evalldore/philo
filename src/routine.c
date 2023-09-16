/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/09/16 18:06:50 by niceguy          ###   ########.fr       */
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
	return (can);
}

static bool	eat(t_philo *philo)
{
	while (!can_pickup(philo))
	{
		if (!ph_is_alive(philo))
			break;
	}
	if (!ph_check_simulation(philo))
	{
		pthread_mutex_lock(philo->pickup);
		philo->forks[0]->check = true;
		philo->forks[1]->check = true;
		pthread_mutex_unlock(philo->pickup);
		return (false);
	}
	pthread_mutex_lock(&philo->forks[0]->mutex);
	ph_print(philo, MSG_FORK);
	pthread_mutex_lock(&philo->forks[1]->mutex);
	ph_print(philo, MSG_FORK);
	philo->last_meal = get_time(philo->rules.start);
	philo->num_meals++;
	ph_print(philo, MSG_EAT);
	ph_sleep(philo, philo->rules.time_to_eat);
	pthread_mutex_unlock(&philo->forks[1]->mutex);
	pthread_mutex_unlock(&philo->forks[0]->mutex);
	pthread_mutex_lock(philo->pickup);
	philo->forks[0]->check = true;
	philo->forks[1]->check = true;
	pthread_mutex_unlock(philo->pickup);
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
	if (philo->num_philos < 2)
	{
		ph_sleep(philo, philo->rules.time_to_die);
		ph_print(philo, MSG_DIED);
		return (NULL);
	}
	if ((philo->id % 2) == 0)
		ph_sleep(philo, philo->rules.time_to_eat);
	while (ph_check_simulation(philo))
	{
		if (!eat(philo) || is_satisfied(philo))
			return (NULL);
		if (!ph_check_simulation(philo))
			return (false);
		ph_print(philo, MSG_SLEEP);
		ph_sleep(philo, philo->rules.time_to_sleep);
		if (!ph_check_simulation(philo))
			return (NULL);
		ph_print(philo, MSG_THINK);
	}
	return (NULL);
}
