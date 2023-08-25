/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 06:32:35 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/25 18:49:11 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int32_t	ft_atoi(const char *str)
{
	int32_t		m;
	int32_t		num;

	if (!str)
		return (0);
	m = 1;
	num = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str++ == '-')
			m *= -1;
	}
	while (*str >= '0' && *str <= '9')
		num = (num * 10) + (*str++ - '0');
	return (num * m);
}

uint64_t	get_time(uint64_t start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000) - start);
}

static bool	ft_isdigit(char	*arg)
{
	while (*arg)
	{
		if (*arg < 48 || *arg > 57)
			return (false);
		arg++;
	}
	return (true);
}

bool	ft_check_args(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (false);
	argv++;
	while (*argv)
	{
		if (!ft_isdigit(*argv))
			return (false);
		argv++;
	}
	return (true);
}
