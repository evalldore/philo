/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:55:33 by evallee-          #+#    #+#             */
/*   Updated: 2023/09/18 18:05:18 by evallee-         ###   ########.fr       */
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
	bool		is_alive;

	pthread_mutex_lock(&philo->lock);
	t = get_time(philo->rules.start) - philo->last_meal;
	is_alive = t < philo->rules.time_to_die;
	pthread_mutex_unlock(&philo->lock);
	return (is_alive);
}

void	ph_print(t_philo *philo, char *msg)
{
	pthread_mutex_lock(philo->print);
	printf(msg, get_time(philo->rules.start), philo->id);
	pthread_mutex_unlock(philo->print);
}

void	ph_clear(t_state *state)
{
	uint32_t	i;

	forks_clear(&state->forks, state->num_philos);
	i = 0;
	while (i < state->num_philos)
		pthread_mutex_destroy(&state->philos[i++].lock);
	if (state->philos)
		free(state->philos);
	if (state->threads)
		free(state->threads);
}
