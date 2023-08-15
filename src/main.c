/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:17:36 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/15 14:20:56 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ph_create_threads(t_philo_state *state)
{
	t_philo_thread	*pt;
	uint32_t		i;

	i = 0;
	state->threads = malloc(sizeof(t_philo_thread) * state->num_philos);
	if (!state->threads)
		return (false);
	while (i < state->num_philos)
	{
		pt = &state->threads[i];
		if (pthread_create(&pt->pthread, NULL, ph_routine, state) < 0)
			return (false);
		i++;
	}
	return (true);
}

static bool	ph_create_philos(t_philo_state *state)
{
	uint32_t	i;

	state->philos = malloc(sizeof(t_philo) * state->num_philos);
	if (!state->philos)
		return (false);
	i = 0;
	while (i < state->num_philos)
	{
		state->philos[i].id = i + 1;
		state->philos[i].last_meal = get_time(state->start_time);
		state->philos[i].num_meals = 0;
		i++;
	}
	return (true);
}

static bool	ph_init(t_philo_state *state, int argc, char **argv)
{
	state->num_philos = (uint32_t)ft_atoi(argv[1]);
	state->time_to_die = (uint32_t)ft_atoi(argv[2]);
	state->time_to_eat = (uint32_t)ft_atoi(argv[3]);
	state->time_to_sleep = (uint32_t)ft_atoi(argv[4]);
	if (state->num_philos == 0 || state->time_to_die == 0||
	state->time_to_eat == 0 || state->time_to_sleep == 0)
		return (false);
	state->num_eats = 0;
	if (argc == 6)
		state->num_eats = (uint32_t)ft_atoi(argv[5]);
	state->start_time = get_time(0);
	if (!forks_init(&(state->forks), state->num_philos))
		return (false);
	if (!ph_create_philos(state))
		return (false);
	state->simulating = true;
	if (pthread_mutex_init(&state->meals, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&state->death, NULL) != 0)
		return (false);
	if (!ph_create_threads(state))
		return (false);
	return (true);
}

static void	ph_simulate(t_philo_state *state)
{
	uint32_t		i;
	bool			ate_all;

	while (state->simulating)
	{
		i = 0;
		ate_all = true;
		pthread_mutex_lock(&state->meals);
		pthread_mutex_lock(&state->death);
		while (i < state->num_philos)
		{
			if ((get_time(state->start_time) - state->philos[i].last_meal) >= state->time_to_die)
			{
				state->simulating = false;
				printf(MSG_DIED, get_time(state->start_time), state->philos[i].id);
				break ;
			}
			if (state->philos[i++].num_meals != state->num_eats)
				ate_all = false;
		}
		if (state->num_eats > 0 && ate_all)
			state->simulating = false;
		pthread_mutex_unlock(&state->meals);
		pthread_mutex_unlock(&state->death);
	}
}

int	main(int argc, char **argv)
{
	t_philo_state	state;
	uint32_t		i;

	if (argc < 5 || argc > 6)
		return (1);
	if (!ph_init(&state, argc, argv))
		return (1);
	ph_simulate(&state);
	i = 0;
	while (i < state.num_philos)
	{
		state.threads[i].ret = pthread_join(state.threads[i].pthread, NULL);
		i++;
	}
	forks_clear(&state.forks, state.num_philos);
	free(state.philos);
	free(state.threads);
	pthread_mutex_destroy(&state.meals);
	pthread_mutex_destroy(&state.death);
	return (0);
}
