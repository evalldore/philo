/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:13:58 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/22 13:25:39 by evallee-         ###   ########.fr       */
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
		pthread_mutex_init(&(*forks)[num++], NULL);
	return (true);
}

void	forks_clear(pthread_mutex_t **forks, uint32_t num_forks)
{
	uint32_t	i;

	i = 0;
	while (i < num_forks)
		pthread_mutex_destroy(&(*forks)[i++]);
	free(*forks);
	*forks = NULL;
}
