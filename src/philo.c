/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:55:33 by evallee-          #+#    #+#             */
/*   Updated: 2023/08/30 19:06:55 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ph_is_simulating(t_philo *philo)
{
	bool		simulating;

	pthread_mutex_lock(philo->death);
	simulating = *philo->simulating;
	pthread_mutex_unlock(philo->death);
	return (simulating);
}

bool	ph_is_alive(t_philo *philo)
{
	uint64_t	t;

	t = get_time(philo->rules.start) - philo->last_meal;
	return (t < philo->rules.time_to_die);
}

bool	ph_check_simulation(t_philo *philo)
{
	if (!ph_is_simulating(philo))
		return (false);
	if (!ph_is_alive(philo))
	{
		if (!ph_is_simulating(philo))
			return (false);
		pthread_mutex_lock(philo->death);
		ph_print(philo, MSG_DIED);
		*philo->simulating = false;
		pthread_mutex_unlock(philo->death);
		return (false);
	}
	return (true);
}

void	ph_print(t_philo *philo, char *msg)
{
	pthread_mutex_lock(philo->print);
	printf(msg, get_time(philo->rules.start), philo->id);
	pthread_mutex_unlock(philo->print);
}

void	ph_clear(t_state *state)
{
	forks_clear(&state->forks, state->num_philos);
	if (state->philos)
		free(state->philos);
	if (state->threads)
		free(state->threads);
}

