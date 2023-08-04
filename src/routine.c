/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/04 05:12:53 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	eat(t_philo_state *state, t_philo *philo)
{
	uint32_t		next_fork;
	
	next_fork = philo->id % state->num_philos;
	pthread_mutex_lock(&state->forks[philo->id - 1]);
	if (!state->simulating)
		return (false);
	printf(MSG_FORK, get_time(state->start_time), philo->id);
	pthread_mutex_lock(&state->forks[next_fork]);
	if (!state->simulating)
		return (false);
	printf(MSG_FORK, get_time(state->start_time), philo->id);
	pthread_mutex_lock(&state->meals);
	if (!state->simulating)
		return (false);
	printf(MSG_EAT, get_time(state->start_time), philo->id);
	philo->last_meal = get_time(state->start_time);
	philo->num_meals++;
	pthread_mutex_unlock(&state->meals);
	usleep(state->time_to_eat * 1000);
	pthread_mutex_unlock(&state->forks[philo->id - 1]);
	pthread_mutex_unlock(&state->forks[next_fork]);
	return (state->simulating);
}

void	*philo_routine(void *ptr)
{
	static uint32_t	index;
	t_philo_state	*state;
	t_philo			*philo;
	uint32_t		next_fork;
	
	state = ptr;
	philo = &state->philos[index++];
	next_fork = philo->id % state->num_philos;
	while (state->simulating)
	{
		if (!eat(state, philo))
		{
			pthread_mutex_unlock(&state->forks[philo->id - 1]);
			pthread_mutex_unlock(&state->forks[next_fork]);
			return (NULL);
		}
		printf(MSG_SLEEP, get_time(state->start_time), philo->id);
		usleep(state->time_to_sleep * 1000);
		if (!state->simulating)
			return (NULL);
		printf(MSG_THINK, get_time(state->start_time), philo->id);
	}
	return (NULL);
}
