/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:17:36 by niceguy           #+#    #+#             */
/*   Updated: 2023/09/18 18:00:32 by evallee-         ###   ########.fr       */
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
		ph->simulating = &state->simulating;
		ph->rules = state->rules;
		ph->id = i + 1;
		ph->last_meal = get_time(state->rules.start);
		ph->num_meals = 0;
		ph->print = &state->print;
		ph->death = &state->death;
		ph->num_philos = state->num_philos;
		if (pthread_mutex_init(&ph->lock, NULL) != 0)
			return (false);
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
	if (!forks_init(&state->forks, state->num_philos))
		return (false);
	if (!ph_create_philos(state))
		return (false);
	if (!ph_create_threads(state))
		return (false);
	return (true);
}

static void	simulate(t_state *state)
{
	uint32_t	i;

	while (state->simulating && !ph_all_satisfied(state))
	{
		i = 0;
		while (i < state->num_philos)
		{
			if (ph_is_satisfied(&state->philos[i]))
			{
				i++;
				continue ;
			}
			if (!ph_is_alive(&state->philos[i]))
			{
				pthread_mutex_lock(&state->death);
				state->simulating = false;
				ph_print(&state->philos[i], MSG_DIED);
				pthread_mutex_unlock(&state->death);
				return ;
			}
			i++;
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
	i = 0;
	simulate(&state);
	while (i < state.num_philos)
		pthread_join(state.threads[i++], NULL);
	ph_clear(&state);
	pthread_mutex_destroy(&state.print);
	pthread_mutex_destroy(&state.death);
	return (0);
}
