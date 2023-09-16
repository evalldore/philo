/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:55:33 by evallee-          #+#    #+#             */
/*   Updated: 2023/09/10 19:28:59 by niceguy          ###   ########.fr       */
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
	if (!ph_is_alive(philo) && ph_is_simulating(philo))
	{
		pthread_mutex_lock(philo->death);
		*philo->simulating = false;
		ph_print(philo, MSG_DIED);
		pthread_mutex_unlock(philo->death);
		return (false);
	}
	if (!ph_is_simulating(philo))
		return (false);
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

