/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   satisfied.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:46:56 by evallee-          #+#    #+#             */
/*   Updated: 2023/09/18 17:52:39 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ph_is_satisfied(t_philo *philo)
{
	bool	satisfied;

	if (philo->rules.num_eats == 0)
		return (false);
	satisfied = false;
	pthread_mutex_lock(&philo->lock);
	if (philo->num_meals >= philo->rules.num_eats)
		satisfied = true;
	pthread_mutex_unlock(&philo->lock);
	return (satisfied);
}

bool	ph_all_satisfied(t_state *state)
{
	uint32_t	i;

	i = 0;
	while (i < state->num_philos)
	{
		if (!ph_is_satisfied(&state->philos[i++]))
			return (false);
	}
	return (true);
}
