/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:17:36 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/26 16:51:10 by niceguy          ###   ########.fr       */
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

	state->philos = malloc(sizeof(t_philo) * state->num_philos);
	if (!state->philos)
		return (false);
	i = 0;
	while (i < state->num_philos)
	{
		ph = &(state->philos[i]);
		ph->simulating = true;
		ph->rules = state->rules;
		ph->id = i + 1;
		ph->last_meal = get_time(state->rules.start);
		ph->num_meals = 0;
		ph->death = &state->death;
		ph->print = &state->print;
		ph->meals = &state->meals;
		ph->num_philos = state->num_philos;
		if ((i % 2) == 0)
		{
			ph->forks[0] = &state->forks[i];
			ph->forks[1] = &state->forks[(i + 1) % state->num_philos];
		}
		else
		{
			ph->forks[0] = &state->forks[(i + 1) % state->num_philos];
			ph->forks[1] = &state->forks[i];
		}
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
	if (pthread_mutex_init(&state->death, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&state->print, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&state->meals, NULL) != 0)
		return (false);
	if (!forks_init(&state->forks, state->num_philos))
		return (false);
	if (!ph_create_philos(state))
		return (false);
	if (!ph_create_threads(state))
		return (false);
	return (true);
}

static void	ph_simulate(t_state *s)
{
	uint32_t		i;
	uint64_t		curr_time;

	while (s->simulating)
	{
		i = 0;
		while (i < s->num_philos)
		{
			curr_time = get_time(s->rules.start);
			pthread_mutex_lock(&s->meals);
			if ((curr_time - s->philos[i].last_meal) > s->rules.time_to_die)
			{
				pthread_mutex_unlock(&s->meals);
				ph_terminate(s);
				printf(MSG_DIED, curr_time, s->philos[i].id);
				break ;
			}
			else
				pthread_mutex_unlock(&s->meals);
		}
	}
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
	ph_simulate(&state);
	i = 0;
	while (i < state.num_philos)
		pthread_join(state.threads[i++], NULL);
	ph_clear(&state);
	pthread_mutex_destroy(&state.death);
	pthread_mutex_destroy(&state.print);
	pthread_mutex_destroy(&state.meals);
	return (0);
}
