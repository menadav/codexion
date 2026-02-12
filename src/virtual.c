/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtual.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:22:33 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/10 19:11:05 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void *monitor_routine(void *arg)
{
    t_data *data;
    int     i;

    data = (t_data *)arg;
    while (1)
    {
        i = 0;
        while (i < data->number_of_coders)
        {
            pthread_mutex_lock(&data->coders[i].compile_mutex);
            if (get_time_ms() - data->coders[i].last_compile_start > data->time_to_burnout)
            {
                pthread_mutex_lock(&data->death_lock);
                data->stop_sim = 1;
                pthread_mutex_unlock(&data->death_lock);
                pthread_mutex_lock(&data->write_lock);
                printf("%lld %d died\n", get_time_ms() - data->start_time, data->coders[i].id);
                pthread_mutex_unlock(&data->write_lock);
                pthread_mutex_unlock(&data->coders[i].compile_mutex);
                return (NULL);
            }
            pthread_mutex_unlock(&data->coders[i].compile_mutex);
            i++;
        }
        usleep(1000);
    }
}


void	compile_action(t_coder *coder)
{
	if ((coder->id % 2) != 0)
	{
		pthread_mutex_lock(&coder->l_dg->dongl);
		pthread_mutex_lock(&coder->r_dg->dongl);
	}
	else
	{
		pthread_mutex_lock(&coder->r_dg->dongl);
		pthread_mutex_lock(&coder->l_dg->dongl);
	}
	pthread_mutex_lock(&coder->compile_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->compile_mutex);
	usleep(coder->data->time_to_compile * 1000);
	pthread_mutex_unlock(&coder->l_dg->dongl);
	pthread_mutex_unlock(&coder->r_dg->dongl);
	coder->compiles_done++;
}

void	debug_action(t_coder *coder)
{
	usleep(coder->data->time_to_debug * 1000);
}

void	refactor_action(t_coder *coder)
{
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
			break;
		}
        	pthread_mutex_unlock(&coder->data->death_lock);
		compile_action(coder);
		debug_action(coder);
		refactor_action(coder);
		printf("Coder %d terminó de compilar y está debugueando\n", coder->id);
		if (coder->compiles_done == coder->data->number_of_compiles_required)
			break ;
	}
	return (NULL);
}

void	*create_threads(t_coder *coders, t_data *data)
{
	int	i;
	pthread_t monitor;

	i = 0;
	data->start_time = get_time_ms();
	while(i < data->number_of_coders)
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

void virtual_data(t_data *data)
{

	data->coders = virtual_coders(data);
	create_threads(data->coders, data);
}
