/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:13:58 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/02 04:22:12 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forks.h"

bool	forks_init(pthread_mutex_t **forks, uint32_t num_forks)
{
	uint32_t	num;

	*forks = malloc(sizeof(pthread_mutex_t) * num_forks);
	if (!(*forks))
		return (false);
	num = 0;
	while (num < num_forks)
	{
		if (pthread_mutex_init(&(*forks)[num], NULL) != 0)
			return (false);
		num++;
	}
	return (true);
}

bool	forks_set(pthread_mutex_t **forks, bool state, uint32_t index, uint32_t num_forks)
{
	uint32_t	next_index;

	if (index >= num_forks)
		return (false);
	next_index = index + 1;
	if (next_index >= num_forks)
		next_index = 0;
	if (state)
	{
		if (pthread_mutex_lock(&(*forks)[index]) != 0)
			return (false);
		if (pthread_mutex_lock(&(*forks)[next_index]) != 0)
			return (false);
		return (true);
	}
	if (pthread_mutex_unlock(&(*forks)[index]) != 0)
		return (false);
	if (pthread_mutex_unlock(&(*forks)[next_index]) != 0)
		return (false);
	return (true);
}
