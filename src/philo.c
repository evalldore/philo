/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:55:33 by evallee-          #+#    #+#             */
/*   Updated: 2023/08/27 06:05:03 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ph_is_alive(t_philo *philo)
{
	bool	simulating;

	pthread_mutex_lock(&philo->lock);
	simulating = philo->simulating;
	pthread_mutex_unlock(&philo->lock);
	return (simulating);
}

void	ph_print(t_philo *philo, char *msg, uint64_t time)
{
	pthread_mutex_lock(philo->print);
	printf(msg, time, philo->id);
	pthread_mutex_unlock(philo->print);
}

void	ph_sleep(t_philo *philo, uint64_t delay)
{
	uint64_t	start;

	start = get_time(philo->rules.start);
	while (ph_is_alive(philo) && (get_time(philo->rules.start) - start) < delay)
		usleep(50);
}

void	ph_clear(t_state *state)
{
	uint32_t	i;
	forks_clear(&state->forks, state->num_philos);
	if (state->philos)
	{
		i = 0;
		while (i < state->num_philos)
			pthread_mutex_destroy(&state->philos[i++].lock);
		free(state->philos);
	}
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
		pthread_mutex_lock(&s->philos[i].lock);
		s->philos[i].simulating = false;
		pthread_mutex_unlock(&s->philos[i].lock);
		i++;
	}
}
