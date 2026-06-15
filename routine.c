/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:25 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/15 13:55:12 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	take_dongles(t_coder *coder, long priority)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (coder->id % 2 != 0)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (!request_dongle(coder, first, priority))
		return (0);
	print_status(coder, "has taken a dongle");
	if (!request_dongle(coder, second, priority))
		return (release_dongle(first, 0), 0);
	print_status(coder, "has taken a dongle");
	return (1);
}

static int	coder_action(t_coder *coder)
{
	pthread_mutex_lock(&coder->time_mutex);
	coder->last_compile_time = get_time();
	pthread_mutex_unlock(&coder->time_mutex);
	print_status(coder, "is compiling");
	ft_usleep(coder->data->time_to_compile, coder);
	release_dongle(coder->left_dongle, coder->data->dongle_cooldown);
	release_dongle(coder->right_dongle, coder->data->dongle_cooldown);
	if (check_death(coder))
		return (1);
	print_status(coder, "is debugging");
	ft_usleep(coder->data->time_to_debug, coder);
	if (check_death(coder))
		return (1);
	print_status(coder, "is refactoring");
	ft_usleep(coder->data->time_to_refactor, coder);
	return (0);
}

static void	*single_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->lock);
	print_status(coder, "has taken a dongle");
	while (!check_death(coder))
		ft_usleep(1, coder);
	pthread_mutex_unlock(&coder->left_dongle->lock);
	return (NULL);
}

static int	routine_loop(t_coder *coder)
{
	long	priority;

	if (coder->data->number_of_compiles_required > 0
		&& coder->compiles_done >= coder->data->number_of_compiles_required)
		return (1);
	priority = get_time();
	if (coder->data->scheduler == 1)
		priority = coder->last_compile_time + coder->data->time_to_burnout;
	if (!take_dongles(coder, priority))
		return (1);
	if (coder_action(coder))
		return (1);
	pthread_mutex_lock(&coder->time_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->time_mutex);
	return (0);
}

void	*routine(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	if (coder->data->number_of_coders == 1)
		return (single_coder(coder));
	if (coder->id % 2 == 0)
		ft_usleep(1, coder);
	while (!check_death(coder))
	{
		if (routine_loop(coder))
			break ;
	}
	return (NULL);
}
