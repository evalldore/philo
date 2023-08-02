/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:17:05 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/02 04:17:53 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FORKS_H
# define FORKS_H
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <pthread.h>

bool	forks_init(pthread_mutex_t **forks, uint32_t num_forks);
bool	forks_set(pthread_mutex_t **forks, bool state, uint32_t index, uint32_t num_forks);

#endif