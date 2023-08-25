/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:55:33 by evallee-          #+#    #+#             */
/*   Updated: 2023/08/25 18:52:37 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ph_is_alive(t_philo *philo)
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

void	ph_print(t_philo *philo, char *msg, uint64_t time)
{
	pthread_mutex_lock(philo->print);
	printf(msg, time, philo->id);
	pthread_mutex_unlock(philo->print);
}

bool	ph_sleep(t_philo *philo, uint64_t delay)
{
	uint64_t	start;

	start = get_time(philo->rules.start);
	while ((get_time(philo->rules.start) - start) < delay)
		usleep(1000);
	return (ph_is_alive(philo));
}

void	ph_clear(t_state *state)
{
	forks_clear(&state->forks, state->num_philos);
	if (state->philos)
		free(state->philos);
	if (state->threads)
		free(state->threads);
}

void	ph_terminate(t_state *s)
{
	uint32_t	i;

	i = 0;
	s->simulating = false;
	while (i < s->num_philos)
		s->philos[i++].simulating = false;
}
