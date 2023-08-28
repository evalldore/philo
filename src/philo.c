/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:55:33 by evallee-          #+#    #+#             */
/*   Updated: 2023/08/28 18:42:04 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ph_is_alive(t_philo *philo)
{
	bool		simulating;
	uint64_t	t;

	pthread_mutex_lock(philo->death);
	simulating = *philo->simulating;
	pthread_mutex_unlock(philo->death);
	if (!simulating)
		return (false);
	t = get_time(philo->rules.start) - philo->last_meal;
	if (t > philo->rules.time_to_die)
	{
		pthread_mutex_lock(philo->death);
		*philo->simulating = false;
		pthread_mutex_unlock(philo->death);
		ph_print(philo, MSG_DIED, get_time(philo->rules.start));
		return (false);
	}
	return (true);
}

void	ph_print(t_philo *philo, char *msg, uint64_t time)
{
	pthread_mutex_lock(philo->print);
	printf(msg, time, philo->id);
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

