/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:13:58 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/01 08:50:32 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "forks.h"

bool	forks_init(bool **forks, uint32_t num_forks)
{
	uint32_t	num;

	*forks = malloc(sizeof(bool) * num_forks);
	if (!(*forks))
		return (false);
	num = 0;
	while (num < num_forks)
	{
		(*forks)[num] = true;
		num++;
	}
	return (true);
}

void	forks_set(bool **forks, bool state, uint32_t index, uint32_t num_forks)
{
	uint32_t	next_index;

	if (index >= num_forks)
		return;
	next_index = index + 1;
	if (next_index >= num_forks)
		next_index = 0;
	(*forks)[index] = state;
	(*forks)[next_index] = state;
}

int32_t	forks_get_available(bool *forks, uint32_t num_forks)
{
	uint32_t	left_index;
	uint32_t	right_index;

	if (num_forks < 2)
		return (-1);
	left_index = 0;
	while (left_index < num_forks)
	{
		if (forks[left_index])
		{
			right_index = left_index + 1;
			if (right_index == num_forks)
				right_index = 0;
			if (forks[right_index])
				return (left_index);
		}
		left_index++;
	}
	return (-1);
}