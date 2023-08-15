/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/15 16:52:16 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_simulate(t_philo_state *state)
{
	pthread_mutex_lock(&state->death);
	if (!state->simulating)
	{
		pthread_mutex_unlock(&state->death);
		return (false);
	}
	pthread_mutex_unlock(&state->death);
	return (true);
}

static bool	pickup_fork(t_philo_state *state, uint32_t pid, uint32_t fork_index)
{
	pthread_mutex_lock(&state->forks[fork_index]);
	if (!check_simulate(state))
	{
		pthread_mutex_unlock(&state->forks[fork_index]);
		return (false);
	}
	printf(MSG_FORK, get_time(state->start), pid);
	return (true);
}

static bool	ready(t_philo_state *state, uint32_t philo_id)
{
	uint32_t		next_fork;

	next_fork = philo_id % state->num_philos;
	if ((philo_id % 2) == 0)
	{
		if (!pickup_fork(state, philo_id, philo_id - 1))
			return (false);
		if (!pickup_fork(state, philo_id, next_fork))
			return (false);
		return (true);
	}
	if (!pickup_fork(state, philo_id, next_fork))
		return (false);
	if (!pickup_fork(state, philo_id, philo_id - 1))
		return (false);
	return (true);
}

static bool	eat(t_philo_state *state, t_philo *philo)
{
	uint32_t		next_fork;

	next_fork = philo->id % state->num_philos;
	ready(state, philo->id);
	if (!check_simulate(state))
		return (false);
	pthread_mutex_lock(&state->meals);
	if (!check_simulate(state))
		return (false);
	printf(MSG_EAT, get_time(state->start), philo->id);
	philo->last_meal = get_time(state->start);
	philo->num_meals++;
	pthread_mutex_unlock(&state->meals);
	usleep(state->time_to_eat * 1000);
	pthread_mutex_unlock(&state->forks[philo->id - 1]);
	pthread_mutex_unlock(&state->forks[next_fork]);
	return (check_simulate(state));
}

void	*ph_routine(void *ptr)
{
	static atomic_int	index;
	t_philo_state		*state;
	t_philo				*philo;

	state = ptr;
	philo = &state->philos[index++];
	while (check_simulate(state))
	{
		if (!eat(state, philo))
			return (NULL);
		printf(MSG_SLEEP, get_time(state->start), philo->id);
		usleep(state->time_to_sleep * 1000);
		if (!check_simulate(state))
			return (NULL);
		printf(MSG_THINK, get_time(state->start), philo->id);
	}
	return (NULL);
}
