/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 11:53:49 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/12 15:17:15 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_coder	t_coder;
typedef struct s_dongle
{
	pthread_mutex_t		dongl;
	long long			available_cldw;
	int					is_taken;
	//future heap for edf t_heap
}	t_dongle;
//===================================================//
typedef struct s_data
{
	long				number_of_coders;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	long				number_of_compiles_required;
	long				dongle_cooldown;
	long				scheduler;
	t_dongle			*dongles;
	t_coder				*coders;
	long long			start_time;
	int					stop_sim;
	pthread_mutex_t		death_lock;
	pthread_mutex_t		write_lock;
}	t_data;
//===================================================//
typedef struct s_coder
{
	pthread_t		thread_id;
	int				id;
	int				compiles_done;
	long			last_compile_start;
	t_data			*data;
	pthread_mutex_t	compile_mutex;
	t_dongle		*l_dg;
	t_dongle		*r_dg;
}	t_coder;
//===================================================//
//====================codexion=======================//
long			ft_atol(const char *str);
int				ft_strcmp(char *s1, char *s2);
int				check_number(char *str);
int				check_type(char *av);
int				check_positiv(char **av);
t_data			*parse(char **av);
//===================================================//
//======================data_create==================//
t_data			*data_create(long *tokens);
//===================================================//
//======================dongle_create==================//
t_dongle		*dongles_array(long max);
int				create_dongle(t_dongle *dongles, long max);
//===================================================//
//======================dongle_utils=================//
long long		get_time_in_ms(void);
void			release_dongle(t_dongle *dongle, t_data *data);
int				can_take_dongle(t_dongle *dongle);
//===================================================//
//===================== actions =====================//
void			compile_action(t_coder *coder);
void			debug_action(t_coder *coder);
void			refactor_action(t_coder *coder);
//===================================================//
//======================virtual======================//
void			create_coder(t_data *data, t_coder *coder, int i);
t_coder			*virtual_coders(t_data *data);
void			virtual_data(t_data *data);
void			*create_threads(t_coder *coders, t_data *data);
void			*coder_routine(void *arg);
//===================================================//
//=====================Monitoring====================//
int				compiles_end(t_data *data, long end);
int				monitor_burnout(t_data *data);
void			*monitor_routine(void *arg);
//===================================================//
int				write_status(t_coder *coder, char *status);
#endif
