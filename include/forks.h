/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:17:05 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/16 10:14:19 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FORKS_H
# define FORKS_H
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdatomic.h>

bool	forks_init(atomic_bool **forks, uint32_t num_forks);

#endif