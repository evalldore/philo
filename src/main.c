/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:17:36 by niceguy           #+#    #+#             */
/*   Updated: 2023/09/11 17:04:28 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ph_create_threads(t_state *s)
{
	uint32_t		i;

	i = 0;
	s->threads = malloc(sizeof(pthread_t) * s->num_philos);
	if (!s->threads)
		return (false);
	while (i < s->num_philos)
	{
		if (pthread_create(&s->threads[i], NULL, ph_routine, &s->philos[i]) < 0)
			return (false);
		i++;
	}
	return (true);
}

static bool	ph_create_philos(t_state *state)
{
	t_philo		*ph;
	uint32_t	i;
	uint64_t	t;

	state->philos = malloc(sizeof(t_philo) * state->num_philos);
	if (!state->philos)
		return (false);
	i = 0;
	t = get_time(state->rules.start);
	while (i < state->num_philos)
	{
		ph = &(state->philos[i]);
		ph->simulating = &state->simulating;
		ph->rules = state->rules;
		ph->id = i + 1;
		ph->last_meal = t;
		ph->num_meals = 0;
		ph->print = &state->print;
		ph->death = &state->death;
		ph->pickup = &state->pickup;
		ph->num_philos = state->num_philos;
		forks(state->forks, ph->forks, i, state->num_philos);
		i++;
	}
	return (true);
}

static bool	ph_init(t_state *state, int argc, char **argv)
{
	if (!ft_check_args(argc, argv))
		return (false);
	state->num_philos = ft_atoi(argv[1]);
	state->rules.time_to_die = ft_atoi(argv[2]);
	state->rules.time_to_eat = ft_atoi(argv[3]);
	state->rules.time_to_sleep = ft_atoi(argv[4]);
	state->rules.num_eats = 0;
	if (argc == 6)
		state->rules.num_eats = ft_atoi(argv[5]);
	state->rules.start = get_time(0);
	state->simulating = true;
	if (pthread_mutex_init(&state->print, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&state->death, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&state->pickup, NULL) != 0)
		return (false);
	if (!forks_init(&state->forks, state->num_philos))
		return (false);
	if (!ph_create_philos(state))
		return (false);
	if (!ph_create_threads(state))
		return (false);
	return (true);
}

int	main(int argc, char **argv)
{
	t_state			state;
	uint32_t		i;

	if (argc < 5 || argc > 6)
		return (1);
	state.forks = NULL;
	state.threads = NULL;
	state.philos = NULL;
	if (!ph_init(&state, argc, argv))
	{
		ph_clear(&state);
		return (1);
	}
	i = 0;
	while (i < state.num_philos)
		pthread_join(state.threads[i++], NULL);
	ph_clear(&state);
	pthread_mutex_destroy(&state.print);
	pthread_mutex_destroy(&state.death);
	pthread_mutex_destroy(&state.pickup);
	return (0);
}
