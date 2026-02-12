/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtual.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:22:33 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/12 15:18:30 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile_action(t_coder *coder)
{
	if ((coder->id % 2) != 0)
	{
		pthread_mutex_lock(&coder->l_dg->dongl);
		write_status(coder, "has taken left dongle");
		pthread_mutex_lock(&coder->r_dg->dongl);
		write_status(coder, "has taken right dongle");
	}
	else
	{
		pthread_mutex_lock(&coder->r_dg->dongl);
		write_status(coder, "has taken right dongle");
		pthread_mutex_lock(&coder->l_dg->dongl);
		write_status(coder, "has taken left dongle");
	}
	pthread_mutex_lock(&coder->compile_mutex);
	coder->last_compile_start = get_time_in_ms();
	pthread_mutex_unlock(&coder->compile_mutex);
	write_status(coder, "is compiling");
	usleep(coder->data->time_to_compile * 1000);
	pthread_mutex_unlock(&coder->l_dg->dongl);
	pthread_mutex_unlock(&coder->r_dg->dongl);
	coder->compiles_done++;
}

void	debug_action(t_coder *coder)
{
	write_status(coder, "is debugging");
	usleep(coder->data->time_to_debug * 1000);
}

void	refactor_action(t_coder *coder)
{
	write_status(coder, "is refactoring");
	usleep(coder->data->time_to_refactor * 1000);	
}

void	*coder_routine(void *arg)
{
	t_coder *coder;
 
	coder = (t_coder *)arg;
	while (1)
	{
		pthread_mutex_lock(&coder->data->death_lock);
		if (coder->data->stop_sim)
		{
			pthread_mutex_unlock(&coder->data->death_lock);
			break ;
		}
        pthread_mutex_unlock(&coder->data->death_lock);
		compile_action(coder);
		debug_action(coder);
		refactor_action(coder);
		if (coder->compiles_done == coder->data->number_of_compiles_required)
			break ;
	}
	return (NULL);
}

void	*create_threads(t_coder *coders, t_data *data)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	data->start_time = get_time_in_ms();
	while (i < data->number_of_coders)
	{
		coders[i].last_compile_start = data->start_time;
		pthread_create(&(coders[i].thread_id), NULL, &coder_routine, &coders[i]);
		i++;
	}
	pthread_create(&monitor, NULL, &monitor_routine, data);
	i = 0;
	while (i < coders[0].data->number_of_coders)
	{
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	return (NULL);
}

void	virtual_data(t_data *data)
{
	data->coders = virtual_coders(data);
	create_threads(data->coders, data);
}
