/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:09 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/10 12:23:27 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_dongle_req
{
	int		coder_id;
	long	priority;
}	t_dongle_req;

typedef struct s_dongle
{
	int					is_taken;
	long				cooldown_end;
	pthread_mutex_t		lock;
	pthread_cond_t		cond;
	t_dongle_req		*heap_data;
	int					heap_size;
}	t_dongle;

typedef struct s_data
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				scheduler;
	long			start_time;
	int				is_dead;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	log_mutex;
	t_dongle		*dongles;
}	t_data;

typedef struct s_coder
{
	int				id;
	int				compiles_done;
	long			last_compile_time;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_mutex_t	time_mutex;
	t_data			*data;
}	t_coder;

long	get_time(void);
int		parse_arg(t_data *data, int argc, char **argv);
void	*routine(void *args);
void	*monitor_routine(void *args);
int		init_coders(t_data *data, t_coder **coders);
int		request_dongle(t_coder *coder, t_dongle *dongle, long priority);
void	release_dongle(t_dongle *dongle, long cooldown_ms);
void	free_system(t_data *data, t_coder *coders, pthread_t *threads);
void	print_status(t_coder *coder, char *status);
int		check_death(t_coder *coder);
void	push_to_heap(t_dongle *dongle, int coder_id, long priority);
int		peek_heap_coder_id(t_dongle *dongle);
void	pop_from_heap(t_dongle *dongle);

#endif