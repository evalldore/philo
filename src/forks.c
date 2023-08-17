/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:13:58 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/16 10:12:51 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forks.h"

bool	forks_init(atomic_bool **forks, uint32_t num_forks)
{
	uint32_t	num;

	*forks = malloc(sizeof(bool) * num_forks);
	if (!(*forks))
		return (false);
	num = 0;
	while (num < num_forks)
		(*forks)[num++] = true;
	return (true);
}
