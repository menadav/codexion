/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:08:16 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/13 13:28:37 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	create_dongle(t_dongle *dongles, long max, long cldw)
{
	long	i;

	i = 0;
	while (i < max)
	{
		if (pthread_mutex_init(&dongles[i].dongl, NULL) != 0)
			return (1);
		dongles[i].available_cldw = 0;
		dongles[i].is_taken = 0;
		dongles[i].cldw = cldw;
		i++;
	}
	return (0);
}

t_dongle	*dongles_array(long max, long cldw)
{
	t_dongle	*dongles;

	dongles = (t_dongle *)malloc(sizeof(t_dongle) * max);
	if (!dongles)
		return (NULL);
	if (create_dongle(dongles, max, cldw) != 0)
		return (free(dongles), NULL);
	return (dongles);
}
