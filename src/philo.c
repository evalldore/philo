/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:55:33 by evallee-          #+#    #+#             */
/*   Updated: 2023/08/26 23:06:29 by niceguy          ###   ########.fr       */
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
		usleep(150);
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
	{
		pthread_mutex_lock(&s->death);
		s->philos[i++].simulating = false;
		pthread_mutex_unlock(&s->death);
	}
}
