/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:06:19 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/13 13:46:45 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_in_ms(void)
{
	struct	timeval tv;
	long	final;

	final = gettimeofday(&tv, NULL);
	if (final)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}


void	take_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongl);
	dongle->available_cldw = get_time_in_ms() + dongle->cldw;
	dongle->is_taken = 0;
	pthread_mutex_unlock(&dongle->dongl);
}

int	can_take_dongle(t_dongle *dongle)
{
	long	now;
	int		succes;

	succes = 0;
	pthread_mutex_lock(&dongle->dongl);
	now = get_time_in_ms();
	if (dongle->is_taken == 0 && now >= dongle->available_cldw)
	{
		dongle->is_taken = 1;
		succes = 1;
	}
	pthread_mutex_unlock(&dongle->dongl);
	return (succes);
}
